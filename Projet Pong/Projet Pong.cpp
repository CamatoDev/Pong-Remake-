#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

//Prototype des fonctions
bool ButtonCollision(float curseur_x, float curseur_y, sf::RectangleShape box, float box_width, float box_height);
//bool BallCollision(sf::RectangleShape player, sf::CircleShape ball, float player_width, float rayon);
bool BallCollision2(sf::RectangleShape player, sf::CircleShape ball, float player_width, float player_height, float rayon);
bool BallCollision3(sf::RectangleShape player, sf::CircleShape ball, float player_width, float player_height, float rayon);
void GoalPlayer1(sf::CircleShape& ball, int& goal1);
void GoalPlayer2(sf::CircleShape& ball, int& goal2);
void LoadFont();
void LoadSourceSound(sf::SoundBuffer& buffer, sf::String phath);
void WriteInWindow(sf::Text& text, sf::String str, float size);
void WriteInWindow(sf::Text& text, sf::String str);
void LoadTexture(sf::Texture &texture, sf::String path);
void LoadTexture(sf::Texture &texture, sf::String path, float x, float y, float width, float heigth);
//Menu
void EndGame();
//Option
void Option();

//Option du jeu 
bool b_vs_player = false; //Option joueur contre joueur 
bool b_easy_IA = false; //option joueur contre une IA facile à battre 
bool b_perfect_IA = true; //option joueur contre une IA invincible 

//score joueur 1 et 2
int scorePlayer1 = 0;
int scorePlayer2 = 0;
char temp1[3];
char temp2[3];

//taille de la fenêtre 
unsigned int width = 800;
unsigned int height = 600;

//police d'écriture 
sf::Font font;
//Pour le son
sf::SoundBuffer mainMenuSoundbuffer;
sf::SoundBuffer BallSoundbuffer;



//lancement du jeu 
void GameStart() {
	//definition de la fenêtre principale du jeu 
	sf::RenderWindow mainWindow(sf::VideoMode(width, height), "CAM PING : GAME");

	//chargement de la texture de l'arrière plan 
	sf::Texture backgroundTexture;
	LoadTexture(backgroundTexture, "data/terrain4.jpg");
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	//background.setTextureRect(sf::IntRect(0, 0, width, height));
	background.setScale(width / 270.f, height / 186.f);

	//initialisation
	float player_width = 20.f;
	float player_heigth = 120.f;
	float player_mass = 10.f;
	// Player 1 
	sf::RectangleShape player1;
	player1.setOrigin(player_width / 2.f, player_heigth / 2.f);
	player1.setSize(sf::Vector2f(player_width, player_heigth));
	player1.setFillColor(sf::Color::Blue);
	player1.setPosition(sf::Vector2f(player_width, height / 2.f));

	// Player 2 (IA)
	sf::RectangleShape player2;
	player2.setOrigin(player_width / 2.f, player_heigth / 2.f);
	player2.setSize(sf::Vector2f(player_width, player_heigth));
	player2.setFillColor(sf::Color::Red);
	player2.setPosition(sf::Vector2f(width - player_width, height / 2.f));

	//création de la balle 
	float rayon = 25.f;
	/*float ball_mass = 5.f;*/
	sf::CircleShape ball(rayon);
	ball.setRadius(rayon);
	ball.setOrigin(rayon, rayon);
	ball.setFillColor(sf::Color::Yellow);
	ball.setPosition(width / 2.f, height / 2.f);

	//création de paramètres de la balle 
	sf::Vector2f ballSpeed;
	float speed_const = -2.5f;
	ballSpeed.x = speed_const;
	ballSpeed.y = speed_const;

	//création de paramètres de joueur 2 (IA)  
	sf::Vector2f speedAI;
	float speed_const_AI = 5.f;
	speedAI.x = 0;
	//si le joueur choisi l'option IA facile à battre 
	if(b_easy_IA)
		speedAI.y = speed_const_AI;
	

	//création de paramètres de joueur 1
	sf::Vector2f playerSpeed;
	float player_speed_const = 5.f;
	playerSpeed.x = 0;

	//création d'une horloge 
	sf::Clock clock;
	sf::Clock clock2;

	//chargement police
	LoadFont();

	//chargement du son
	LoadSourceSound(BallSoundbuffer, "res/ball.wav");
	sf::Sound ballSound;
	ballSound.setBuffer(BallSoundbuffer);

	//définition de la variable qui vas contenir le score 
	float sizeScoreText = 40.f;
	sf::Text score1;
	score1.setPosition(width / 2.f, 40.f);
	WriteInWindow(score1, "0", sizeScoreText);
	sf::Text score2;
	score2.setPosition(width / 2.f + 40, 40.f);
	WriteInWindow(score2, "0", sizeScoreText);
	

	//vaeiable pour mettre le jeu sur pause 
	bool pause = false;

	while (mainWindow.isOpen())
	{
		float dt = clock2.restart().asSeconds();

		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			//création de l'option pause
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					pause = !pause;
				}

				//déplacement joueur 
				if (event.key.code == sf::Keyboard::Z) {
					playerSpeed.y = -player_speed_const;
				}
				if (event.key.code == sf::Keyboard::S) {
					playerSpeed.y = player_speed_const;
				}
				
				//Si le match est en joueur contre joueur 
				if (b_vs_player) {
					if (event.key.code == sf::Keyboard::Up) {
						speedAI.y = -speed_const_AI;
					}
					if (event.key.code == sf::Keyboard::Down) {
						speedAI.y = speed_const_AI;
					}
				}
				
			}
			if (event.type == sf::Event::KeyReleased) {
				//déplacement joueur 
				if (event.key.code == sf::Keyboard::Z) {
					playerSpeed.y = 0;
				}
				if (event.key.code == sf::Keyboard::S) {
					playerSpeed.y = 0;
				}
				//Si le match est en joueur contre joueur 
				if (b_vs_player) {
					//déplacement joueur2
					if (event.key.code == sf::Keyboard::Up) {
						speedAI.y = 0;
					}
					if (event.key.code == sf::Keyboard::Down) {
						speedAI.y = 0;
					}
				}
				
			}


		}

		//boucle de jeu 
		if (!pause) {
			//on verifie que la balle ne sort pas de la fenêtre 

			//collision en Y (haut et bas)
			if (ball.getPosition().y + rayon + ballSpeed.y >= height || ball.getPosition().y - rayon + ballSpeed.y <= 0) {
				ballSpeed.y = -ballSpeed.y;
				ballSound.play();
			}

			//collision avec le joueur1 (gauche) 
			if (BallCollision3(player1, ball, player_width, player_heigth, rayon)) {
				//si la ball est à la moitié inférieur de la palette 
				if (ball.getPosition().y > player1.getPosition().y) {
					//la balle monte 
					ballSpeed.x = -ballSpeed.x;
					ballSpeed.y = -ballSpeed.y;
				}
				else {
					//la balle descend
					ballSpeed.x = -ballSpeed.x;
					ballSpeed.y = ballSpeed.y;
				}
				ballSound.play();
				/*ballSpeed = -ballSpeed;*/
			}

			//collision avec le joueur2 ou l'IA (Droite)
			if (BallCollision2(player2, ball, player_width, player_heigth, rayon)) {
				//si la ball est à la moitié inférieur de la palette 
				if (ball.getPosition().y > player2.getPosition().y) {
					//la balle monte 
					ballSpeed.x = -ballSpeed.x;
					ballSpeed.y = -ballSpeed.y;
				}
				else {
					//la balle descend
					ballSpeed.x = -ballSpeed.x;
					ballSpeed.y = ballSpeed.y;
				}
				ballSound.play();
				/*ballSpeed = -ballSpeed;*/
			}


			//but 1
			if (ball.getPosition().x + rayon + ballSpeed.x <= 0) {
				GoalPlayer1(ball, scorePlayer2);
				sprintf_s(temp1, "%d", scorePlayer2);
				WriteInWindow(score2, temp1, sizeScoreText);
			}
			//but 2 
			if (ball.getPosition().x - rayon + ballSpeed.x >= width) {
				GoalPlayer2(ball, scorePlayer1);
				sprintf_s(temp2, "%d", scorePlayer1);
				WriteInWindow(score1, temp2, sizeScoreText);
			}

			//on vérifie les scores 
			if (scorePlayer1 >= 10 || scorePlayer2 >= 10) {
				//si l'un des deux joueurs atteint 10 points on ferme la fenêtre 
				mainWindow.close();
			}

			//Si le match est en joueur contre joueur ou joueur contre IA facile 
			if (b_vs_player || b_easy_IA) {
				//on vérifie que les joueur ne sortent pas de la fenètre 
				if (player2.getPosition().y + player_heigth / 2.f + speedAI.y >= height || player2.getPosition().y - player_heigth / 2.f + speedAI.y <= 0) {
					speedAI.y = -speedAI.y;
				}
			}
			


			//Déplacement de la balle dans la fenêtre de jeu 
			if (clock.getElapsedTime().asMilliseconds() > 1000.f / 60.f) {
				ball.move(ballSpeed);
				player1.move(playerSpeed);
				//Si le match est en joueur contre joueur ou joueur contre IA facile 
				if(b_vs_player || b_easy_IA)
					player2.move(speedAI);
				//si le match est en joueur contre perfect IA 
				if(b_perfect_IA)
					//ici le joueur2 est une IA impatable 
					player2.setPosition(player2.getPosition().x, ball.getPosition().y);
				clock.restart();
			}
		}

		//Bloquer le joueur1 dans la fenêtre 
		if (player1.getPosition().y - player_heigth / 2.f + playerSpeed.y <= 0) {
			player1.setPosition(sf::Vector2f(player1.getPosition().x, player_heigth / 2.f - playerSpeed.y));
		}
		if (player1.getPosition().y + player_heigth / 2.f + playerSpeed.y >= height) {
			player1.setPosition(sf::Vector2f(player1.getPosition().x, height - player_heigth / 2.f - playerSpeed.y));
		}

		//affichage fenêtre 
		mainWindow.clear();
		mainWindow.draw(background);
		mainWindow.draw(ball);
		mainWindow.draw(player1);
		mainWindow.draw(player2);
		mainWindow.draw(score1);
		mainWindow.draw(score2);
		mainWindow.display();
	}
}

void MainMenu() {
	//Définition de la fenêtre du menu principale 
	sf::RenderWindow mainMenu(sf::VideoMode(width, height), "CAM PING : MENU");

	//chargement de la texture de l'arrière plan 
	sf::Texture backgroundTexture;
	LoadTexture(backgroundTexture, "data/neon.jpg");
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	//background.setTextureRect(sf::IntRect(0, 0, width, height));
	background.setScale(width / 474.f, height / 266.f);

	//Boutton 
	sf::RectangleShape play, option, quit;
	float button_width = 220.f;
	float button_height = 80.f;
	float offset = 10.f;

	//chargement police
	LoadFont();

	//chargement du song 
	LoadSourceSound(mainMenuSoundbuffer, "res/doodle_pop.ogg");
	sf::Sound music;
	music.setBuffer(mainMenuSoundbuffer);
	music.setLoop(true);
	music.play();

	//pour le boutton play 
	play.setOrigin(button_width / 2.f, button_height / 2.f);
	play.setSize(sf::Vector2f(button_width, button_height));
	play.setPosition(sf::Vector2f(width / 2.f, height / 2.f));
	play.setFillColor(sf::Color::Cyan);
	sf::Text playText;
	playText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f);
	WriteInWindow(playText, "PLAY");

	//pour le boutton option 
	option.setOrigin(button_width / 2.f, button_height / 2.f);
	option.setSize(sf::Vector2f(button_width, button_height));
	option.setPosition(sf::Vector2f(width / 2.f, height / 2.f + offset + button_height));
	option.setFillColor(sf::Color::Cyan);
	sf::Text optionText;
	optionText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f + offset + button_height);
	WriteInWindow(optionText, "OPTION");

	//pour le boutton quitter 
	quit.setOrigin(button_width / 2.f, button_height / 2.f);
	quit.setSize(sf::Vector2f(button_width, button_height));
	quit.setPosition(sf::Vector2f(width / 2.f, height / 2.f + button_height * 2.f + offset * 2.f));
	quit.setFillColor(sf::Color::Cyan);
	sf::Text quitText;
	quitText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f + button_height * 2.f + offset * 2.f);
	WriteInWindow(quitText, "QUIT");

	//gestion du menu principal 
	while (mainMenu.isOpen()) {
		sf::Event events;
		while (mainMenu.pollEvent(events)) {
			if (events.type == sf::Event::Closed)
				mainMenu.close();

			//Touche souris PLAY
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, play, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						mainMenu.close();
						//lacer le jeu principal
						GameStart();
					}
				}
			}
			//Touche souris OPTION
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, option, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						mainMenu.close();
						//lacer le menu des options
						Option();
					}
				}
			}
			//Touche souris QUIT
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, quit, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						//quitter le jeu 
						mainMenu.close();
					}
				}
			}
			//Touche clavier  
			if (events.type == sf::Event::KeyPressed) {
				if (events.key.code == sf::Keyboard::Enter) {
					mainMenu.close();
					//lacer le jeu principal 
					GameStart();
				}
			}
		}

		mainMenu.clear();
		mainMenu.draw(background);
		mainMenu.draw(play);
		mainMenu.draw(playText);
		mainMenu.draw(option);
		mainMenu.draw(optionText);
		mainMenu.draw(quit);
		mainMenu.draw(quitText);
		mainMenu.display();
	}
}

void Option() {
	//Définition de la fenêtre du d'option du jeu 
	sf::RenderWindow optionWindow(sf::VideoMode(width, height), "CAM PING : OPTION");

	//chargement de la texture de l'arrière plan 
	sf::Texture backgroundTexture;
	LoadTexture(backgroundTexture, "data/neon.jpg");
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	//background.setTextureRect(sf::IntRect(0, 0, width, height));
	background.setScale(width / 474.f, height / 266.f);

	//Boutton 
	sf::RectangleShape vs_player, easy_IA, perfect_IA;
	float button_width = 220.f;
	float button_height = 80.f;
	float offset = 10.f;

	//chargement police
	LoadFont();

	//pour le boutton play 
	vs_player.setOrigin(button_width / 2.f, button_height / 2.f);
	vs_player.setSize(sf::Vector2f(button_width, button_height));
	vs_player.setPosition(sf::Vector2f(width / 2.f, height / 2.f));
	vs_player.setFillColor(sf::Color::Cyan);
	sf::Text vs_playerText;
	vs_playerText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f);
	WriteInWindow(vs_playerText, "OFF");
	sf::Text vs_playerText2;
	vs_playerText2.setPosition(width / 2.f - button_width / 2.f - 280.f, height / 2.f - button_height / 2.f);
	WriteInWindow(vs_playerText2, "Player 2");

	//pour le boutton option 
	easy_IA.setOrigin(button_width / 2.f, button_height / 2.f);
	easy_IA.setSize(sf::Vector2f(button_width, button_height));
	easy_IA.setPosition(sf::Vector2f(width / 2.f, height / 2.f + offset + button_height));
	easy_IA.setFillColor(sf::Color::Cyan);
	sf::Text easy_IAText;
	easy_IAText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f + offset + button_height);
	WriteInWindow(easy_IAText, "OFF");
	sf::Text easy_IAText2;
	easy_IAText2.setPosition(width / 2.f - button_width / 2.f - 280.f, height / 2.f - button_height / 2.f + offset + button_height);
	WriteInWindow(easy_IAText2, "Easy IA");

	//pour le boutton quitter 
	perfect_IA.setOrigin(button_width / 2.f, button_height / 2.f);
	perfect_IA.setSize(sf::Vector2f(button_width, button_height));
	perfect_IA.setPosition(sf::Vector2f(width / 2.f, height / 2.f + button_height * 2.f + offset * 2.f));
	perfect_IA.setFillColor(sf::Color::Cyan);
	sf::Text perfect_IAText;
	perfect_IAText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f + button_height * 2.f + offset * 2.f);
	WriteInWindow(perfect_IAText, "ON");
	sf::Text perfect_IAText2;
	perfect_IAText2.setPosition(width / 2.f - button_width / 2.f - 280.f, height / 2.f - button_height / 2.f + button_height * 2.f + offset * 2.f);
	WriteInWindow(perfect_IAText2, "Perfect");

	//On appui sur ENTRER pour un retour au menu
	sf::Text instroction;
	WriteInWindow(instroction, "Back to menu : 'ENTRER'");
	//instroction.setFillColor(sf::Color::Cyan);

	//gestion du menu principal 
	while (optionWindow.isOpen()) {
		sf::Event events;
		while (optionWindow.pollEvent(events)) {
			if (events.type == sf::Event::Closed)
				optionWindow.close();

			//Touche souris PLAY
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, vs_player, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						b_vs_player = !b_vs_player;
						if(b_vs_player)
							WriteInWindow(vs_playerText, "ON");
						else
							WriteInWindow(vs_playerText, "OFF");
					}
				}
			}
			//Touche souris OPTION
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, easy_IA, button_width, button_height)) {
						b_easy_IA = !b_easy_IA;
						if (b_easy_IA)
							WriteInWindow(easy_IAText, "ON");
						else
							WriteInWindow(easy_IAText, "OFF");
					}
				}
			}
			//Touche souris QUIT
			if (events.type == sf::Event::MouseButtonPressed) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, perfect_IA, button_width, button_height)) {
						b_perfect_IA = !b_perfect_IA;
						if (b_perfect_IA)
							WriteInWindow(perfect_IAText, "ON");
						else
							WriteInWindow(perfect_IAText, "OFF");
					}
				}
			}
			//Touche clavier ENTRER pour revenir au menu  
			if (events.type == sf::Event::KeyPressed) {
				if (events.key.code == sf::Keyboard::Enter) {
					optionWindow.close();
					//lacer le jeu principal 
					MainMenu();
				}
			}
		}

		optionWindow.clear();
		optionWindow.draw(background);
		optionWindow.draw(instroction);
		optionWindow.draw(vs_player);
		optionWindow.draw(vs_playerText2);
		optionWindow.draw(vs_playerText);
		optionWindow.draw(easy_IA);
		optionWindow.draw(easy_IAText2);
		optionWindow.draw(easy_IAText);
		optionWindow.draw(perfect_IA);
		optionWindow.draw(perfect_IAText2);
		optionWindow.draw(perfect_IAText);
		optionWindow.display();
	}
}

void EndGame() {
	//création de la fenêtre de fin de jeu 
	sf::RenderWindow endWindow(sf::VideoMode(width, height), "CAM PING : END");

	//chargement de la texture de l'arrière plan 
	sf::Texture backgroundTexture;
	LoadTexture(backgroundTexture, "data/simple.jpg");
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	//background.setTextureRect(sf::IntRect(0, 0, width, height));
	background.setScale(width / 234.f, height / 234.f);

	//chargement de la police d'écriture 
	LoadFont();
	//Boutton 
	sf::RectangleShape retry, menu;
	float button_width = 220.f;
	float button_height = 80.f;
	float offset = 10.f;

	//pour le boutton play 
	retry.setOrigin(button_width / 2.f, button_height / 2.f);
	retry.setSize(sf::Vector2f(button_width, button_height));
	retry.setPosition(sf::Vector2f(width / 2.f, height / 2.f));
	retry.setFillColor(sf::Color::Cyan);
	sf::Text retryText;
	retryText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f);
	WriteInWindow(retryText, "RETRY");

	//pour le boutton option 
	menu.setOrigin(button_width / 2.f, button_height / 2.f);
	menu.setSize(sf::Vector2f(button_width, button_height));
	menu.setPosition(sf::Vector2f(width / 2.f, height / 2.f + offset + button_height));
	menu.setFillColor(sf::Color::Cyan);
	sf::Text menuText;
	menuText.setPosition(width / 2.f - button_width / 2.f, height / 2.f - button_height / 2.f + offset + button_height);
	WriteInWindow(menuText, "MENU");

	//resultat de la partie 
	sf::Text result;

	if (scorePlayer1 > scorePlayer2) {
		WriteInWindow(result, "PLAYER BLUE WIN : " + std::to_string(scorePlayer1) + " | " + std::to_string(scorePlayer2));
	}
	else {
		WriteInWindow(result, "PLAYER RED WIN : " + std::to_string(scorePlayer1) + " | " + std::to_string(scorePlayer2));
	}

	//gestion de la fenêtre de fin 
	while (endWindow.isOpen()) {
		sf::Event event;
		while (endWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				endWindow.close();

			//Touche souris RETRY
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(event.mouseButton.x + button_width / 2, event.mouseButton.y + button_height / 2, retry, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						endWindow.close();
						//lacer le jeu principal
						GameStart();
					}
				}
			}

			//Touche souris MENU
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (ButtonCollision(event.mouseButton.x + button_width / 2, event.mouseButton.y + button_height / 2, menu, button_width, button_height)) {
						/*astuce debug std::cout << "ok" << std::endl;*/
						endWindow.close();
						//lacer le menu principal 
						MainMenu();
					}
				}
			}
			endWindow.clear();
			endWindow.draw(background);
			endWindow.draw(retry);
			endWindow.draw(retryText);
			endWindow.draw(menu);
			endWindow.draw(menuText);
			endWindow.draw(result);
			endWindow.display();
		}
	}
}

int main()
{
	MainMenu();
	return 0;
}

//collission avec les bouttons
bool ButtonCollision(float curseur_x, float curseur_y, sf::RectangleShape box, float box_width, float box_height)
{
	if (curseur_x >= box.getPosition().x
		&& curseur_x < box.getPosition().x + box_width
		&& curseur_y >= box.getPosition().y
		&& curseur_y < box.getPosition().y + box_height)
		return true;
	else
		return false;
}

/*non utilisé 
bool BallCollision(sf::RectangleShape player, sf::CircleShape ball, float player_width, float rayon) {
	float col_x1 = player.getPosition().x - ball.getPosition().x;
	float col_y1 = player.getPosition().y - ball.getPosition().y;
	float dist_player_ball = (player_width) + (rayon);

	//check 
	if ((col_x1 * col_x1 + col_y1 * col_y1) <= dist_player_ball * dist_player_ball) {
		return true;
	}
	return false;
}
*/

//collision entre la balle et le joueur2
bool BallCollision2(sf::RectangleShape player, sf::CircleShape ball, float player_width, float player_height, float rayon) {
	//check 
	if (ball.getPosition().x + rayon > player.getPosition().x - player_width &&
		ball.getPosition().x + rayon < player.getPosition().x &&
		ball.getPosition().y + rayon >= player.getPosition().y - player_height / 2.f &&
		ball.getPosition().y - rayon <= player.getPosition().y + player_height / 2.f)
		return true;
	else
		return false;
}

//collision entre la balle et le joueur1
 bool BallCollision3(sf::RectangleShape player, sf::CircleShape ball, float player_width, float player_height, float rayon) {
	//check 
	if (ball.getPosition().x - rayon < player.getPosition().x + player_width &&
		ball.getPosition().x - rayon > player.getPosition().x &&
		ball.getPosition().y + rayon >= player.getPosition().y - player_height / 2.f &&
		ball.getPosition().y - rayon <= player.getPosition().y + player_height / 2.f)
		return true;
	else
		return false;
}




void GoalPlayer1(sf::CircleShape& ball, int& goal1) {
	//repositionnement
	ball.setPosition(width / 2.f, height / 2.f);
	goal1 += 1;
	std::cout << "Player 2 : " << goal1 << "\n";
	//La partie ce termine si le joueur2 atteint 10 point 
	if (goal1 >= 10) {
		goal1 = 10;
		//appelle de la fenêtre de fin
		EndGame();
	}

}

void GoalPlayer2(sf::CircleShape& ball, int& goal2) {
	//repositionnement
	ball.setPosition(width / 2.f, height / 2.f);
	goal2 += 1;
	std::cout << "Player 1 : " << goal2 << "\n";
	//La partie ce termine si le joueur1 atteint 10 point 
	if (goal2 >= 10) {
		goal2 = 10;
		//appelle de la fenêtre de fin	
		EndGame();
	}
}

void LoadFont() {
	if (!font.loadFromFile("res/Poppins-Regular.ttf")) {
		std::cout << "Erreur chargement de la police d'écriture." << std::endl; 
	}
}

void LoadSourceSound(sf::SoundBuffer& buffer, sf::String phath) {
	if (!buffer.loadFromFile(phath)) {
		std::cout << "Erreur chargement su son" << std::endl;
	}
}

void WriteInWindow(sf::Text &text, sf::String str, float size) {
	text.setFont(font);
	text.setOrigin(size / 2, size / 2);
	//text.setPosition(width / 2.f, 40.f);
	/*concatener avec des int pour actualiser le score*/
	text.setString(str);

	text.setCharacterSize(size);
	text.setFillColor(sf::Color::Blue);
	text.setStyle(sf::Text::Bold);
}
void WriteInWindow(sf::Text &text, sf::String str) {
	text.setFont(font);
	/*concatener avec des int pour actualiser le score*/
	text.setString(str);
	text.setCharacterSize(60);
	text.setFillColor(sf::Color::Green);
	text.setStyle(sf::Text::Bold);
}

void LoadTexture(sf::Texture &texture, sf::String path) {
	if (!texture.loadFromFile(path)) {
		std::cout << "Erreur chargement de la texture." << std::endl;
	}
}
void LoadTexture(sf::Texture &texture, sf::String path, float x, float y, float width, float height) {
	if (!texture.loadFromFile(path, sf::IntRect(x, y, width, height))) {
		std::cout << "Erreur chargement de la texture." << std::endl;
	}
}