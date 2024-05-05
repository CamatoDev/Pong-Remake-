#include <SFML/Graphics.hpp>
#include <iostream>

bool ButtonCollision(float curseur_x, float curseur_y, sf::RectangleShape box, float box_width, float box_height);
bool BallCollision(sf::RectangleShape player, sf::CircleShape ball, float player_width, float rayon);
void GoalPlayer1(sf::CircleShape& ball, int& goal1);
void GoalPlayer2(sf::CircleShape& ball, int& goal2);
void LoadFont();
void WriteInWindow(sf::Text& text, sf::String str, float size);
void WriteInWindow(sf::Text& text, sf::String str);
void LoadTexture(sf::Texture &texture, sf::String path);
void LoadTexture(sf::Texture &texture, sf::String path, float x, float y, float width, float heigth);
//Menu
void EndGame();

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

//lancement du jeu 
void GameStart() {
	//definition de la fenêtre principale du jeu 
	sf::RenderWindow mainWindow(sf::VideoMode(width, height), "CAM PING : GAME");
	//initialisation
	float player_width = 20.f;
	float player_heigth = 150.f;
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
	float ball_mass = 5.f;
	sf::CircleShape ball(rayon);
	ball.setRadius(rayon);
	ball.setOrigin(rayon, rayon);
	ball.setFillColor(sf::Color::Yellow);
	ball.setPosition(width / 2.f, height / 2.f);

	//création de paramètres de la balle 
	sf::Vector2f ballSpeed;
	float speed_const = -2.f;
	ballSpeed.x = speed_const;
	ballSpeed.y = speed_const;

	//création de paramètres de joueur 2 (IA)  
	sf::Vector2f speedAI;
	float speed_const_AI = 5.f;
	speedAI.x = 0;
	//speedAI.y = speed_const_AI;

	//création de paramètres de joueur 1
	sf::Vector2f playerSpeed;
	float player_speed_const = 5.f;
	playerSpeed.x = 0;

	//création d'une horloge 
	sf::Clock clock;
	sf::Clock clock2;

	//chargement police
	LoadFont();

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
				if (event.key.code == sf::Keyboard::Up) {
					speedAI.y = -speed_const_AI;
				}
				if (event.key.code == sf::Keyboard::Down) {
					speedAI.y = speed_const_AI;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				//déplacement joueur 
				if (event.key.code == sf::Keyboard::Z) {
					playerSpeed.y = 0;
				}
				if (event.key.code == sf::Keyboard::S) {
					playerSpeed.y = 0;
				}//déplacement joueur 
				/*if (event.key.code == sf::Keyboard::Up) {
					speedAI.y = 0;
				}
				if (event.key.code == sf::Keyboard::Down) {
					speedAI.y = 0;
				}*/
			}


		}

		//boucle de jeu 
		if (!pause) {
			//on verifie que la balle ne sort pas de la fenêtre 

			//sortie en Y
			if (ball.getPosition().y + rayon + ballSpeed.y >= height || ball.getPosition().y - rayon + ballSpeed.y <= 0) {
				ballSpeed.y = -ballSpeed.y;
			}

			//collision avec le joueur1 
			if (BallCollision(player1, ball, player_width, rayon)) {
				//speed_f = ((m1 - m2)*speed + (2*m2*speed2))/ (m1 + m2);
				//sf::Vector2f speed_ball_f = ((player_mass - ball_mass) * playerSpeed + (2 * ball_mass * ballSpeed)) / (player_mass + ball_mass);

				ballSpeed = -ballSpeed;
			}

			//collision avec le joueur2
			if (BallCollision(player2, ball, player_width, rayon)) {
				std::cout << "ok" << std::endl;
				/*sf::Vector2f speed_ball_f = ((player_mass - ball_mass) * playerSpeed + (2 * ball_mass * ballSpeed)) / (player_mass + ball_mass);*/

				//ballSpeed = -ballSpeed;
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

			//on vérifie que les joueur ne sortent pas de la fenètre 
			if (player2.getPosition().y + player_heigth / 2.f + speedAI.y >= height || player2.getPosition().y - player_heigth / 2.f + speedAI.y <= 0) {
				speedAI.y = -speedAI.y;
			}


			//Déplacement de la balle dans la fenêtre de jeu 
			if (clock.getElapsedTime().asMilliseconds() > 1000.f / 60.f) {
				ball.move(ballSpeed);
				player1.move(playerSpeed);
				player2.move(speedAI);
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

	//Boutton 
	sf::RectangleShape play, option, quit;
	float button_width = 220.f;
	float button_height = 80.f;
	float offset = 10.f;

	//gestion de la fenêtre de fin 
	while (endWindow.isOpen()) {
		sf::Event event;
		while (endWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				endWindow.close();

			endWindow.clear();
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

//collision entre la balle et un joueur 
bool BallCollision(sf::RectangleShape player, sf::CircleShape ball, float player_width, float rayon) {
	float col_x1 = player.getPosition().x - ball.getPosition().x;
	float col_y1 = player.getPosition().y - ball.getPosition().y;
	float dist_player_ball = (player.getPosition().x) + (rayon);

	//check 
	if ((col_x1 * col_x1 + col_y1 * col_y1) <= dist_player_ball * dist_player_ball) {
		return true;
	}
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