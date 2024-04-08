#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>


#define SPEED	2

using std::cout;
using std::cin;


struct Player {
	int gameBoardID = -1;
	std::string Name[11];
	long long Score = 0;
	sf::Sprite* BallSprite = NULL;
	std::string Ball[11];
	bool isWinner = 0;
	bool Active = 0;
	std::map<std::pair<int, int>, int> winPos;
};

struct Move {
	int row = -1;
	int col = -1;
	bool gameEnd = 0;
	bool played = 0;
	int playedBy = -1; // index of the player 
	float posX = -1;
	float posY = -1;
};

bool drawAlert(std::string s, sf::Vector2i alertWindowPos);
bool playOffline(sf::RenderWindow& welcomeWindow, int** gameBoard, int ROWS, int COLS, Player player[], int* posX, std::string& posY);
bool playWindow(int** gameBoard, int ROWS, int COLS, Player player[], int* posX, std::string& posY);
Move drawGameBoard(sf::RenderWindow& window, sf::Sprite& boardSprite, const float Xstart, const float Ystart, int ROWS, int COLS, int** gameBoard, Player player[], bool isWin, int* posX, std::string& posY);
Move animateMove(sf::RenderWindow& window, sf::Sprite& boardSprite, const float Xstart, const float Ystart, int ROWS, int COLS, int** gameBoard, Player player[], Move& move);
bool evaluateWinner(int** gameBoard, int ROWS, int COLS, Player player[], Move move, bool AI);

int main()
{	
	int ROWS = 0, COLS = 0;

	std::string posY;

	// Making Dynamic 1D Array
	int* posX = NULL;
	Player* player = NULL;

	// Making Dynamic 2D Array
	int** gameBoard = NULL;

	// redBall Configuration
	sf::Texture redBallTexture;
	if (!redBallTexture.loadFromFile("images//redBallSmall.png"))
	{
		// error...
	}
	redBallTexture.setSmooth(true);
	sf::Sprite redBallSprite;
	redBallSprite.setTexture(redBallTexture);
	// yellowBall Configuration
	sf::Texture yellowBallTexture;
	if (!yellowBallTexture.loadFromFile("images//yellowBallSmall.png"))
	{
		// error...
	}
	yellowBallTexture.setSmooth(true);
	sf::Sprite yellowBallSprite;
	yellowBallSprite.setTexture(yellowBallTexture);

	bool isClosed = 0;
	bool isClicked = 0;

	sf::Vector2i welcomeWindowDim;
	welcomeWindowDim.x = 900;
	welcomeWindowDim.y = 600;

	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	// Welcome Window
	sf::RenderWindow welcomeWindow(sf::VideoMode(welcomeWindowDim.x, welcomeWindowDim.y, 32), "CONNECT FOUR GAME !", sf::Style::None);
	welcomeWindow.setMouseCursorVisible(false);

	sf::Vector2i welcomeWindowPos((desktopMode.width - welcomeWindowDim.x) / 2, (desktopMode.height - welcomeWindowDim.y) / 2);
	welcomeWindow.setPosition(welcomeWindowPos);


	// background Configuration
	sf::Texture backgroundWelcomeTexture;
	if (!backgroundWelcomeTexture.loadFromFile("images//back10.jpg"))
	{
		// error...
	}
	backgroundWelcomeTexture.setSmooth(true);
	sf::Sprite backgroundWelcomeSprite;
	backgroundWelcomeSprite.setTexture(backgroundWelcomeTexture);
	backgroundWelcomeSprite.setOrigin(sf::Vector2f(0.f, 0.f));

	backgroundWelcomeSprite.setScale(
		welcomeWindowDim.x / backgroundWelcomeSprite.getLocalBounds().width,
		welcomeWindowDim.y / backgroundWelcomeSprite.getLocalBounds().height);


	sf::Clock clock;
	sf::Time elapsedTime = clock.getElapsedTime();

	while (elapsedTime.asSeconds() < 3) {
		elapsedTime = clock.getElapsedTime();
		welcomeWindow.clear();
		welcomeWindow.draw(backgroundWelcomeSprite);
		welcomeWindow.display();
	}

	welcomeWindow.setMouseCursorVisible(true);

	// background Configuration
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("images//back9.jpg"))
	{
		// error...
	}
	backgroundTexture.setSmooth(true);
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sf::Vector2f(0.f, 0.f));

	backgroundSprite.setScale(
		welcomeWindowDim.x / backgroundSprite.getLocalBounds().width,
		welcomeWindowDim.y / backgroundSprite.getLocalBounds().height);



	float Xstart = 600, Ystart = 150;
	sf::Vector2i mousePos;
	// Button Configuration
	sf::Texture buttonTexture;
	if (!buttonTexture.loadFromFile("images//buttonReleasedSmall.png"))
	{
		// error...
	}
	buttonTexture.setSmooth(true);
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(buttonTexture);
	buttonSprite.setOrigin(sf::Vector2f(0.f, 0.f));
	buttonSprite.setPosition(Xstart, Ystart);

	// Button Hover Configuration
	sf::Texture buttonHoverTexture;
	if (!buttonHoverTexture.loadFromFile("images//buttonHoverSmall.png"))
	{
		// error...
	}
	buttonHoverTexture.setSmooth(true);
	sf::Sprite buttonHoverSprite;
	buttonHoverSprite.setTexture(buttonHoverTexture);
	buttonHoverSprite.setOrigin(sf::Vector2f(0.f, 0.f));
	buttonHoverSprite.setPosition(Xstart, Ystart);


	sf::Font textFont;
	if (!textFont.loadFromFile("fonts/mvboli.ttf")) {
		//error
	}

	sf::Text buttonTitle;
	sf::String titleText;

	buttonTitle.setFont(textFont);
	buttonTitle.setCharacterSize(25);
	buttonTitle.setStyle(sf::Text::Bold);
	buttonTitle.setFillColor(sf::Color::Black);


	while (welcomeWindow.isOpen() && !isClosed)
	{
		sf::Event event;
		buttonHoverSprite.setColor(sf::Color(255, 255, 255));

		welcomeWindow.clear();
		welcomeWindow.draw(backgroundSprite);
		buttonSprite.setPosition(Xstart, Ystart);
		welcomeWindow.draw(buttonSprite);

		buttonSprite.setPosition(Xstart, (Ystart + 100));
		welcomeWindow.draw(buttonSprite);

		buttonSprite.setPosition(Xstart, (Ystart + 200));
		welcomeWindow.draw(buttonSprite);

		isClicked = 0;
		while (welcomeWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				welcomeWindow.close();
				isClosed = 1;
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					isClicked = 1;
				}
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 27)
					welcomeWindow.close();
			}
		}
		mousePos = sf::Mouse::getPosition(welcomeWindow);
		if (mousePos.y >= Ystart &&
			mousePos.y < (buttonSprite.getLocalBounds().height + Ystart) &&
			mousePos.x >= Xstart &&
			mousePos.x < (buttonSprite.getLocalBounds().width + Xstart)) {
			if (isClicked) {
				buttonHoverSprite.setColor(sf::Color(255, 255, 255, 128));
				isClicked = 0;
				isClosed = playOffline(welcomeWindow, gameBoard, ROWS, COLS, player, posX, posY);
			}
			else {
				buttonHoverSprite.setPosition(Xstart, Ystart);
				welcomeWindow.draw(buttonHoverSprite);
			}
		}
		else if (mousePos.y >= Ystart + 100 &&
			mousePos.y < (buttonSprite.getLocalBounds().height + Ystart + 100) &&
			mousePos.x >= Xstart &&
			mousePos.x < (buttonSprite.getLocalBounds().width + Xstart)) {
			if (isClicked) {
				buttonHoverSprite.setColor(sf::Color(255, 255, 255, 128));
				isClicked = 0;
			}
			else {
				buttonHoverSprite.setPosition(Xstart, Ystart + 100);
				welcomeWindow.draw(buttonHoverSprite);
			}
		}
		else if (mousePos.y >= Ystart + 200 &&
			mousePos.y < (buttonSprite.getLocalBounds().height + Ystart + 200) &&
			mousePos.x >= Xstart &&
			mousePos.x < (buttonSprite.getLocalBounds().width + Xstart)) {
			if (isClicked) {
				buttonHoverSprite.setColor(sf::Color(255, 255, 255, 128));
				isClicked = 0;
			}
			else {
				buttonHoverSprite.setPosition(Xstart, Ystart + 200);
				welcomeWindow.draw(buttonHoverSprite);
			}
		}

		titleText = "Player vs Player";
		buttonTitle.setString(titleText);
		buttonTitle.setPosition(Xstart + 20, Ystart + 10);
		welcomeWindow.draw(buttonTitle);

		titleText = "Player vs AI";
		buttonTitle.setString(titleText);
		buttonTitle.setPosition(Xstart + 45, (Ystart + 100 + 10));
		welcomeWindow.draw(buttonTitle);

		titleText = "Multi-Player";
		buttonTitle.setString(titleText);
		buttonTitle.setPosition(Xstart + 50, (Ystart + 200 + 10));
		welcomeWindow.draw(buttonTitle);

		welcomeWindow.display();
	}




	return 0;
}


bool playOffline(sf::RenderWindow & welcomeWindow, int** gameBoard, int ROWS, int COLS, Player player[], int* posX, std::string & posY) {
	std::string* str;
	int playersCount = 2;
	bool isNext = 0;
	bool isClicked = 0;
	bool isPlayFirstChecked = 0;
	bool isRedChecked = 1;
	bool inputIsOk = 1;
	int inputActive = 1;


	// redBall Configuration
	sf::Texture redBallTexture;
	if (!redBallTexture.loadFromFile("images//redBallSmall.png"))
	{
		// error...
	}
	redBallTexture.setSmooth(true);
	sf::Sprite redBallSprite;
	redBallSprite.setTexture(redBallTexture);
	// yellowBall Configuration
	sf::Texture yellowBallTexture;
	if (!yellowBallTexture.loadFromFile("images//yellowBallSmall.png"))
	{
		// error...
	}
	yellowBallTexture.setSmooth(true);
	sf::Sprite yellowBallSprite;
	yellowBallSprite.setTexture(yellowBallTexture);

	// redBallSmall Configuration
	sf::Texture redBallSmallTexture;
	if (!redBallSmallTexture.loadFromFile("images//redBallVerySmall.png"))
	{
		// error...
	}
	redBallSmallTexture.setSmooth(true);
	sf::Sprite redBallSmallSprite;
	redBallSmallSprite.setTexture(redBallSmallTexture);
	redBallSmallSprite.setPosition(760, 200);

	// yellowBallSmall Configuration
	sf::Texture yellowBallSmallTexture;
	if (!yellowBallSmallTexture.loadFromFile("images//yellowBallVerySmall.png"))
	{
		// error...
	}
	yellowBallSmallTexture.setSmooth(true);
	sf::Sprite yellowBallSmallSprite;
	yellowBallSmallSprite.setTexture(yellowBallSmallTexture);
	yellowBallSmallSprite.setPosition(840, 200);


	// multiCheckedBox Configuration
	sf::Texture multiCheckedBoxTexture;
	if (!multiCheckedBoxTexture.loadFromFile("images//checkedBoxSmall.png"))
	{
		// error...
	}
	multiCheckedBoxTexture.setSmooth(true);
	sf::Sprite multiCheckedBoxSprite;
	multiCheckedBoxSprite.setTexture(multiCheckedBoxTexture);


	// multiUncheckedBox Configuration
	sf::Texture multiUncheckedBoxTexture;
	if (!multiUncheckedBoxTexture.loadFromFile("images//uncheckedBoxSmall.png"))
	{
		// error...
	}
	multiUncheckedBoxTexture.setSmooth(true);
	sf::Sprite multiUncheckedBoxSprite;
	multiUncheckedBoxSprite.setTexture(multiUncheckedBoxTexture);


	// checkedBox Configuration
	sf::Texture checkedBoxTexture;
	if (!checkedBoxTexture.loadFromFile("images//checkedBoxSmall.png"))
	{
		// error...
	}
	checkedBoxTexture.setSmooth(true);
	sf::Sprite checkedBoxSprite;
	checkedBoxSprite.setTexture(checkedBoxTexture);
	checkedBoxSprite.setPosition(725, 270);

	// uncheckedBox Configuration
	sf::Texture uncheckedBoxTexture;
	if (!uncheckedBoxTexture.loadFromFile("images//uncheckedBoxSmall.png"))
	{
		// error...
	}
	uncheckedBoxTexture.setSmooth(true);
	sf::Sprite uncheckedBoxSprite;
	uncheckedBoxSprite.setTexture(uncheckedBoxTexture);
	uncheckedBoxSprite.setPosition(725, 270);

	// background Configuration
	sf::Texture backgroundWelcomeTexture;
	if (!backgroundWelcomeTexture.loadFromFile("images//back9.jpg"))
	{
		// error...
	}
	backgroundWelcomeTexture.setSmooth(true);
	sf::Sprite backgroundWelcomeSprite;
	backgroundWelcomeSprite.setTexture(backgroundWelcomeTexture);
	backgroundWelcomeSprite.setOrigin(sf::Vector2f(0.f, 0.f));

	backgroundWelcomeSprite.setScale(
		welcomeWindow.getSize().x / backgroundWelcomeSprite.getLocalBounds().width,
		welcomeWindow.getSize().y / backgroundWelcomeSprite.getLocalBounds().height);

	float Xstart = 775, Ystart = 540;
	sf::Vector2i mousePos;
	// Button Configuration
	sf::Texture buttonTexture;
	if (!buttonTexture.loadFromFile("images//buttonReleasedVerySmall.png"))
	{
		// error...
	}
	buttonTexture.setSmooth(true);
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(buttonTexture);
	buttonSprite.setOrigin(sf::Vector2f(0.f, 0.f));
	buttonSprite.setPosition(Xstart, Ystart);

	// Button Hover Configuration
	sf::Texture buttonHoverTexture;
	if (!buttonHoverTexture.loadFromFile("images//buttonHoverVerySmall.png"))
	{
		// error...
	}
	buttonHoverTexture.setSmooth(true);
	sf::Sprite buttonHoverSprite;
	buttonHoverSprite.setTexture(buttonHoverTexture);
	buttonHoverSprite.setOrigin(sf::Vector2f(0.f, 0.f));
	buttonHoverSprite.setPosition(Xstart, Ystart);


	sf::Font textFont;
	if (!textFont.loadFromFile("fonts/mvboli.ttf")) {
		//error
	}


	sf::Text buttonTitle;
	sf::Text windowTitle;
	sf::Text nameText;
	sf::Text nameBallColorText;
	sf::Text namePlayFirstText;
	sf::Text rowsText;
	sf::Text colsText;
	sf::Text name2Text;
	sf::Text input1;
	sf::Text input2;
	sf::Text input3;
	sf::Text input4;
	sf::Text arrow;
	sf::Text alert;

	sf::String titleText;
	sf::String windowTitleText;
	sf::String input1Text;
	sf::String input2Text = '6';
	sf::String input3Text = '7';
	sf::String input4Text;

	buttonTitle.setFont(textFont);
	buttonTitle.setCharacterSize(25);
	buttonTitle.setStyle(sf::Text::Bold);
	buttonTitle.setFillColor(sf::Color::Black);
	titleText = "Next";
	buttonTitle.setString(titleText);

	windowTitle.setFont(textFont);
	windowTitle.setCharacterSize(35);
	windowTitle.setStyle(sf::Text::Bold);
	windowTitle.setFillColor(sf::Color::White);
	windowTitleText = "Players' Info.";
	windowTitle.setString(windowTitleText);
	windowTitle.setPosition(580, 25);

	nameText.setFont(textFont);
	nameText.setCharacterSize(25);
	nameText.setStyle(sf::Text::Bold);
	nameText.setFillColor(sf::Color::Black);
	nameText.setString("Player 1's Name (Max 11)");
	nameText.setPosition(550, 100);

	nameBallColorText.setFont(textFont);
	nameBallColorText.setCharacterSize(25);
	nameBallColorText.setStyle(sf::Text::Bold);
	nameBallColorText.setFillColor(sf::Color::Black);
	nameBallColorText.setString("Ball Color ?");
	nameBallColorText.setPosition(550, 200);

	namePlayFirstText.setFont(textFont);
	namePlayFirstText.setCharacterSize(25);
	namePlayFirstText.setStyle(sf::Text::Bold);
	namePlayFirstText.setFillColor(sf::Color::Black);
	namePlayFirstText.setString("Play First ?");
	namePlayFirstText.setPosition(550, 270);


	rowsText.setFont(textFont);
	rowsText.setCharacterSize(25);
	rowsText.setStyle(sf::Text::Bold);
	rowsText.setFillColor(sf::Color::Black);
	rowsText.setString("Row Size (Max 8): ");
	rowsText.setPosition(550, 320);

	colsText.setFont(textFont);
	colsText.setCharacterSize(25);
	colsText.setStyle(sf::Text::Bold);
	colsText.setFillColor(sf::Color::Black);
	colsText.setString("Col Size (Max 8): ");
	colsText.setPosition(550, 370);

	name2Text.setFont(textFont);
	name2Text.setCharacterSize(25);
	name2Text.setStyle(sf::Text::Bold);
	name2Text.setFillColor(sf::Color::Black);
	name2Text.setString("Player 2's Name (Max 11)");
	name2Text.setPosition(550, 420);


	input1.setFont(textFont);
	input1.setCharacterSize(25);
	input1.setStyle(sf::Text::Bold);
	input1.setFillColor(sf::Color::Black);
	input1.setPosition(630, 150);


	input2.setFont(textFont);
	input2.setCharacterSize(25);
	input2.setStyle(sf::Text::Bold);
	input2.setFillColor(sf::Color::Black);
	input2.setPosition(820, 322);
	input2.setString(input2Text);

	input3.setFont(textFont);
	input3.setCharacterSize(25);
	input3.setStyle(sf::Text::Bold);
	input3.setFillColor(sf::Color::Black);
	input3.setPosition(820, 374);
	input3.setString(input3Text);

	input4.setFont(textFont);
	input4.setCharacterSize(25);
	input4.setStyle(sf::Text::Bold);
	input4.setFillColor(sf::Color::Black);
	input4.setPosition(630, 470);

	arrow.setFont(textFont);
	arrow.setCharacterSize(25);
	arrow.setStyle(sf::Text::Bold);
	arrow.setFillColor(sf::Color::Black);
	arrow.setString("->");
	arrow.setPosition(600, 150);

	alert.setFont(textFont);
	alert.setCharacterSize(20);
	alert.setStyle(sf::Text::Bold);
	alert.setFillColor(sf::Color::Red);
	alert.setString("Names should NOT be empty !");
	alert.setPosition(540, 70);

	while (!isNext && welcomeWindow.isOpen()) {

		sf::Event event;
		buttonHoverSprite.setColor(sf::Color(255, 255, 255));
		buttonSprite.setPosition(Xstart, Ystart);
		welcomeWindow.clear();
		welcomeWindow.draw(backgroundWelcomeSprite);
		welcomeWindow.draw(windowTitle);
		welcomeWindow.draw(nameText);
		welcomeWindow.draw(nameBallColorText);
		welcomeWindow.draw(namePlayFirstText);
		welcomeWindow.draw(rowsText);
		welcomeWindow.draw(colsText);
		welcomeWindow.draw(name2Text);
		welcomeWindow.draw(input1);
		welcomeWindow.draw(input2);
		welcomeWindow.draw(input3);
		welcomeWindow.draw(input4);
		welcomeWindow.draw(redBallSmallSprite);
		welcomeWindow.draw(yellowBallSmallSprite);
		welcomeWindow.draw(arrow);
		if (!inputIsOk)
			welcomeWindow.draw(alert);
		if (isPlayFirstChecked)
			welcomeWindow.draw(checkedBoxSprite);
		else
			welcomeWindow.draw(uncheckedBoxSprite);
		if (isRedChecked) {
			multiCheckedBoxSprite.setPosition(725, 200);
			multiUncheckedBoxSprite.setPosition(805, 200);
			welcomeWindow.draw(multiCheckedBoxSprite);
			welcomeWindow.draw(multiUncheckedBoxSprite);
		}
		else {
			multiCheckedBoxSprite.setPosition(805, 200);
			multiUncheckedBoxSprite.setPosition(725, 200);
			welcomeWindow.draw(multiCheckedBoxSprite);
			welcomeWindow.draw(multiUncheckedBoxSprite);
		}

		isClicked = 0;
		while (welcomeWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				welcomeWindow.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					isClicked = 1;
				}
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode ==  9) {
					if (inputActive == 4)
						inputActive = 1;
					else
						inputActive++;
				}
				if (inputActive == 1) {
					arrow.setPosition(600, 150);
					if ((event.text.unicode >= 'a' && event.text.unicode <= 'z') || (event.text.unicode >= 'A' && event.text.unicode <= 'Z')) {
						if (input1Text.getSize() < 11) {
							input1Text += event.text.unicode;
							input1.setString(input1Text);
						}
					}
					if (event.text.unicode == ' ') {
						if (input1Text.getSize() < 11 && input1Text.getSize() > 0) {
							input1Text += " ";
							input1.setString(input1Text);
						}
					}
					if (event.text.unicode == 8 && input1Text.getSize() > 0) {
						input1Text.erase(input1Text.getSize() - 1);
						input1.setString(input1Text);
					}
				}
				else if (inputActive == 2) {
					arrow.setPosition(520, 320);
					if ((event.text.unicode >= '6' && event.text.unicode <= '8')) {
						if (input2Text.getSize() <= 1) {
							input2Text.erase(input2Text.getSize() - 1);
							input2Text += event.text.unicode;
							input2.setString(input2Text);
						}
					}
				}

				else if (inputActive == 3) {
					arrow.setPosition(520, 370);
					if ((event.text.unicode >= '6' && event.text.unicode <= '8')) {
						if (input3Text.getSize() <= 1) {
							input3Text.erase(input2Text.getSize() - 1);
							input3Text += event.text.unicode;
							input3.setString(input3Text);
						}
					}
				}
				else if (inputActive == 4) {
					arrow.setPosition(600, 470);
					if ((event.text.unicode >= 'a' && event.text.unicode <= 'z') || (event.text.unicode >= 'A' && event.text.unicode <= 'Z')) {
						if (input4Text.getSize() < 11) {
							input4Text += event.text.unicode;
							input4.setString(input4Text);
						}
					}
					if (event.text.unicode == ' ') {
						if (input4Text.getSize() < 11 && input4Text.getSize() > 0) {
							input4Text += " ";
							input4.setString(input4Text);
						}
					}
					if (event.text.unicode == 8 && input4Text.getSize() > 0) {
						input4Text.erase(input4Text.getSize() - 1);
						input4.setString(input4Text);
					}
				}
				if (event.text.unicode == 27)
					welcomeWindow.close();
				//	cout << event.text.unicode << "\n";
			}

		}
		welcomeWindow.draw(buttonSprite);
		mousePos = sf::Mouse::getPosition(welcomeWindow);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	//	cout << mousePos.x << ' ' << mousePos.y << "\n";
		if (multiUncheckedBoxSprite.getGlobalBounds().contains(mousePosF)) {
			if (isClicked) {
				isClicked = 0;
				if (isRedChecked)
					isRedChecked = 0;
				else
					isRedChecked = 1;
			}
		}
		if (multiCheckedBoxSprite.getGlobalBounds().contains(mousePosF)) {
			if (isClicked) {
				isClicked = 0;
				if (isRedChecked)
					isRedChecked = 0;
				else
					isRedChecked = 1;
			}
		}
		if (uncheckedBoxSprite.getGlobalBounds().contains(mousePosF)) {
			if (isClicked) {
				isClicked = 0;
				if (!isPlayFirstChecked) {
					isPlayFirstChecked = 1;
					welcomeWindow.draw(checkedBoxSprite);
				}
				else {
					isPlayFirstChecked = 0;
					welcomeWindow.draw(uncheckedBoxSprite);
				}
			}
		}
		if (mousePos.y >= Ystart &&
			mousePos.y < (buttonSprite.getLocalBounds().height + Ystart) &&
			mousePos.x >= Xstart &&
			mousePos.x < (buttonSprite.getLocalBounds().width + Xstart)) {
			if (isClicked) {
				if (input1Text.getSize() == 0 || input4Text.getSize() == 0)
					inputIsOk = 0;
				else {
					inputIsOk = 1;
					buttonHoverSprite.setColor(sf::Color(255, 255, 255, 128));
					isClicked = 0;
					isNext = 1;
				}
			}
			else {
				buttonHoverSprite.setPosition(Xstart, Ystart);
				welcomeWindow.draw(buttonHoverSprite);
			}
		}

		buttonTitle.setPosition(Xstart + 20, Ystart + 4);
		welcomeWindow.draw(buttonTitle);

		welcomeWindow.display();
	}





	str = new std::string;
	*str = input2.getString();
	ROWS = std::stoi(*str);
	*str = input3.getString();
	COLS = std::stoi(*str);

	delete str;
	str = NULL;


	// Making Dynamic 2D Array
	gameBoard = new int* [ROWS];
	for (int i = 0; i < ROWS; ++i)
		gameBoard[i] = new int[COLS];


	if (isNext) {
		do {
			cout << posY << "\n";

			posY.clear();
			delete[] posX;
			delete[] player;

			player = new Player[playersCount]();
			player[0].gameBoardID = (int)1;
			player[1].gameBoardID = (int)2;

			*player[0].Name = input1Text;
			*player[1].Name = input4Text;

			player[0].Active = isPlayFirstChecked;
			player[1].Active = !isPlayFirstChecked;


			if (isRedChecked) {
				*player[0].Ball = "Red";
				*player[1].Ball = "Yellow";
			}
			else {
				*player[1].Ball = "Red";
				*player[0].Ball = "Yellow";
			}


			if (*player[0].Ball == "Red")
				(player[0].BallSprite) = &redBallSprite;
			else if (*player[0].Ball == "Yellow")
				(player[0].BallSprite) = &yellowBallSprite;

			if (*player[1].Ball == "Red")
				(player[1].BallSprite) = &redBallSprite;
			else if (*player[1].Ball == "Yellow")
				(player[1].BallSprite) = &yellowBallSprite;


			posX = new int[COLS]();

			if (isPlayFirstChecked)
				posY = "0#";
			else
				posY = "1#";

			// Init The Board With Zeros
			for (int i = 0; i < ROWS * COLS; ++i)
				gameBoard[i / COLS][i % COLS] = 0;
		} while (playWindow(gameBoard, ROWS, COLS, player, posX, posY));
	}
	// For Deletion
	posY.clear();

	delete[] posX;
	posX = NULL;

	delete[] player;
	player = NULL;

	for (int i = 0; i < ROWS; ++i)
		delete[] gameBoard[i];
	delete[] gameBoard;
	gameBoard = NULL;

	return 0;
}

bool playWindow(int** gameBoard, int ROWS, int COLS, Player player[], int* posX, std::string & posY) {

	Move move;
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	
	// Play Window
	sf::RenderWindow window(sf::VideoMode(desktopMode.width,
		desktopMode.height,
		desktopMode.bitsPerPixel),
		"CONNECT FOUR GAME !",
		sf::Style::Fullscreen);

	// When a key is pressed, sf::Event::KeyPressed will be true only once
	window.setKeyRepeatEnabled(false);


	// Board Configuration
	sf::Texture boardTexture;
	if (!boardTexture.loadFromFile("images//boardSmall.png"))
	{
		// error...
	}
	boardTexture.setSmooth(true);
	sf::Sprite boardSprite;
	boardSprite.setTexture(boardTexture);


	// background Configuration
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("images//back3.jpg"))
	{
		// error...
	}
	backgroundTexture.setSmooth(true);
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setOrigin(sf::Vector2f(0.f, 0.f));

	backgroundSprite.setScale(
		desktopMode.width / backgroundSprite.getLocalBounds().width,
		desktopMode.height / backgroundSprite.getLocalBounds().height);


	sf::Font textFont;
	if (!textFont.loadFromFile("fonts/mvboli.ttf")) {
		//error
	}

	sf::Text buttonTitle;
	sf::String buttonTitleText;

	buttonTitle.setFont(textFont);
	buttonTitle.setCharacterSize(25);
	buttonTitle.setStyle(sf::Text::Bold);
	buttonTitle.setFillColor(sf::Color::Black);
	buttonTitleText = "Again ? -> Enter";
	buttonTitle.setString(buttonTitleText);
	buttonTitle.setPosition(45, 612);

	// Button Configuration
	sf::Texture buttonTexture;
	if (!buttonTexture.loadFromFile("images//buttonReleasedSmall.png"))
	{
		// error...
	}
	buttonTexture.setSmooth(true);
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(buttonTexture);
	buttonSprite.setOrigin(sf::Vector2f(0.f, 0.f));
	buttonSprite.setPosition(30, 600);





	sf::Text gameTitle;
	sf::Text playerName;
	sf::Text playerScore;
	sf::Text playerActive;
	sf::Text playerBall;
	sf::String titleText;
	sf::String nameText;
	sf::String scoreText;
	sf::String activeText;
	sf::String ballText;
	gameTitle.setFont(textFont);
	gameTitle.setCharacterSize(50);
	gameTitle.setStyle(sf::Text::Bold);
	gameTitle.setFillColor(sf::Color::White);
	titleText = "CONNECT FOUR GUI";
	gameTitle.setString(titleText);
	gameTitle.setPosition(((desktopMode.width / 2) - titleText.getSize() * (gameTitle.getCharacterSize() / 3.05)), 10);
	playerName.setFont(textFont);
	playerName.setCharacterSize(25);
	playerName.setStyle(sf::Text::Bold);
	playerName.setFillColor(sf::Color::White);
	playerScore.setFont(textFont);
	playerScore.setCharacterSize(25);
	playerScore.setStyle(sf::Text::Bold);
	playerScore.setFillColor(sf::Color::White);
	playerActive.setFont(textFont);
	playerActive.setCharacterSize(25);
	playerActive.setStyle(sf::Text::Bold);
	playerActive.setFillColor(sf::Color::White);
	playerBall.setFont(textFont);
	playerBall.setCharacterSize(25);
	playerBall.setStyle(sf::Text::Bold);
	playerBall.setFillColor(sf::Color::White);


	sf::Event event;

	while (window.isOpen())
	{
		//	cout << sf::Mouse::getPosition(window).x << ' ' << sf::Mouse::getPosition(window).y << "\n";
		window.clear();
		window.draw(backgroundSprite);
		window.draw(gameTitle);
		nameText = "Player " + std::to_string(player[0].gameBoardID) + "'s Name : " + *player[0].Name;
		playerName.setString(nameText);
		playerName.setPosition(30, 100);
		scoreText = "Player " + std::to_string(player[0].gameBoardID) + "'s Score : " + std::to_string(player[0].Score);
		playerScore.setString(scoreText);
		playerScore.setPosition(30, 150);
		if (player[0].Active) {
			activeText = "TURN ? -> YES";
		}
		else {
			activeText = "TURN ? -> NO";
		}
		playerActive.setString(activeText);
		playerActive.setPosition(30, 200);
		ballText = "Ball Color : " + *player[0].Ball;
		playerBall.setString(ballText);
		playerBall.setPosition(30, 250);
		window.draw(playerName);
		window.draw(playerScore);
		window.draw(playerActive);
		window.draw(playerBall);

		nameText = "Player " + std::to_string(player[1].gameBoardID) + "'s Name : " + *player[1].Name;
		playerName.setString(nameText);
		playerName.setPosition(30, 350);
		scoreText = "Player " + std::to_string(player[1].gameBoardID) + "'s Score : " + std::to_string(player[1].Score);
		playerScore.setString(scoreText);
		playerScore.setPosition(30, 400);
		if (player[1].Active) {
			activeText = "TURN ? -> YES";
		}
		else {
			activeText = "TURN ? -> NO";
		}
		playerActive.setString(activeText);
		playerActive.setPosition(30, 450);
		ballText = "Ball Color : " + *player[1].Ball;
		playerBall.setString(ballText);
		playerBall.setPosition(30, 500);
		window.draw(playerName);
		window.draw(playerScore);
		window.draw(playerActive);
		window.draw(playerBall);

		if (move.played) {
			move = animateMove(window, boardSprite, 500, 75, ROWS, COLS, gameBoard, player, move);

		}
		else {
			move = drawGameBoard(window, boardSprite, 500, 75, ROWS, COLS, gameBoard, player,
				evaluateWinner(gameBoard, ROWS, COLS, player, move, false), posX, posY);
		}
		if (move.gameEnd) {
			window.draw(buttonSprite);
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode == 27)
						window.close();
					if (event.text.unicode ==  13) {
						move.row = -1;
						move.col = -1;
						move.gameEnd = 0;
						move.played = 0;
						move.playedBy = -1; // index of the player 
						move.posX = -1;
						move.posY = -1;
						return 1;
					}
				}
			}
			window.draw(buttonTitle);
		}
		window.display();
	}


	return 0;
}

Move drawGameBoard(sf::RenderWindow & window, sf::Sprite & boardSprite, const float Xstart, const float Ystart, int ROWS, int COLS, int** gameBoard, Player player[], bool isWin, int* posX, std::string & posY) {
	bool isDraw = 1;
	bool isAlertClosed = 0;
	for (int T = 0; T < COLS; T++) {
		if (posX[T] < ROWS) {
			isDraw = 0;
			break;
		}
	}
	if (isAlertClosed) {
		isDraw = 0;
	}
	// winBall Configuration
	sf::Texture winBallTexture;
	if (!winBallTexture.loadFromFile("images//winSmall.png"))
	{
		// error...
	}
	winBallTexture.setSmooth(true);
	sf::Sprite winBallSprite;
	winBallSprite.setTexture(winBallTexture);
	sf::Vector2i winBallSpritePos;
	Move move;
	sf::Event* mouseClicked = new sf::Event;
	sf::Vector2i mousePos;
	bool isClicked = 0;
	int i, j;
	float X = Xstart, Y = Ystart;
	for (i = ROWS - 1; i >= 0; i--) {
		X = Xstart;
		for (j = 0; j < COLS; j++) {
			boardSprite.setPosition(X, Y);
			if (!isWin && posY[posY.size() - 1] != '$' && !isDraw) {
				mousePos = sf::Mouse::getPosition(window);
				if (mousePos.y >= Ystart &&
					mousePos.y < ((boardSprite.getLocalBounds().height * ROWS) + Ystart) &&
					mousePos.x >= (Xstart + (boardSprite.getLocalBounds().width * j)) &&
					mousePos.x < ((boardSprite.getLocalBounds().width * (j + 1)) + Xstart)) {
					while (window.pollEvent((*mouseClicked))) {
						if ((*mouseClicked).type == sf::Event::TextEntered) {
							if ((*mouseClicked).text.unicode == 27)
								window.close();
						}
						if ((*mouseClicked).type == sf::Event::MouseButtonPressed) {
							if ((*mouseClicked).mouseButton.button == sf::Mouse::Left) {
								isClicked = 1;
							}
						}
					}
					if (posX[j] < ROWS) {
						boardSprite.setColor(sf::Color(255, 255, 255, 128));
						if (isClicked) {
							if (player[0].Active) {
								move.row = posX[j];
								move.col = j;
								move.played = true;
								move.playedBy = 0;
								gameBoard[posX[j]][j] = -1;
								posX[j]++;
								player[0].Active = false;
								player[1].Active = true;
								posY += std::to_string(j + 1);
							}
							else if (player[1].Active) {
								move.row = posX[j];
								move.col = j;
								move.played = true;
								move.playedBy = 1;
								gameBoard[posX[j]][j] = -1;
								posX[j]++;
								player[1].Active = false;
								player[0].Active = true;
								posY += std::to_string(j + 1);
							}
							isClicked = 0;
						}
					}
				}
				else {
					boardSprite.setColor(sf::Color(255, 255, 255));
				}
			}
			else {
				if (posY[posY.size() - 1] != '$') {
					if (isDraw) {
						posY += "!";
						isAlertClosed = drawAlert("Draw", sf::Vector2i(550, 200));
					}
					posY += "$";
				}
				move.gameEnd = 1;
			}
			if (gameBoard[i][j] == player[0].gameBoardID && move.played == 0) {
				(*player[0].BallSprite).setPosition(X, Y);
				window.draw(*player[0].BallSprite);
				if ((player[0].winPos).find(std::make_pair(i, j)) != (player[0].winPos).end() && player[0].isWinner) {
					winBallSpritePos.x = ((*player[0].BallSprite).getGlobalBounds().width - winBallSprite.getGlobalBounds().width) / 2;
					winBallSpritePos.y = ((*player[0].BallSprite).getGlobalBounds().height - winBallSprite.getGlobalBounds().height) / 2;
					winBallSprite.setPosition(X + winBallSpritePos.x, Y + winBallSpritePos.y);
					window.draw(winBallSprite);
				}
			}
			else if (gameBoard[i][j] == player[1].gameBoardID && move.played == 0) {
				(*player[1].BallSprite).setPosition(X, Y);
				window.draw(*player[1].BallSprite);
				if ((player[1].winPos).find(std::make_pair(i, j)) != (player[1].winPos).end() && player[1].isWinner) {
					winBallSpritePos.x = ((*player[1].BallSprite).getGlobalBounds().width - winBallSprite.getGlobalBounds().width) / 2;
					winBallSpritePos.y = ((*player[1].BallSprite).getGlobalBounds().height - winBallSprite.getGlobalBounds().height) / 2;
					winBallSprite.setPosition(X + winBallSpritePos.x, Y + winBallSpritePos.y);
					window.draw(winBallSprite);
				}
			}
			window.draw(boardSprite);
			X += boardSprite.getLocalBounds().width;
		}
		Y += boardSprite.getLocalBounds().height;
	}

	move.posX = (Xstart + (boardSprite.getLocalBounds().width * move.col));
	move.posY = (Ystart - ((boardSprite.getLocalBounds().height) / 2));
	boardSprite.setColor(sf::Color(255, 255, 255));
	delete mouseClicked;
	mouseClicked = NULL;
	return move;
}

Move animateMove(sf::RenderWindow & window, sf::Sprite & boardSprite, const float Xstart, const float Ystart, int ROWS, int COLS, int** gameBoard, Player player[], Move & move) {

	int i, j;
	float X = Xstart, Y = Ystart;
	(*player[move.playedBy].BallSprite).setPosition(move.posX, move.posY);
	move.posY += SPEED;
	window.draw(*player[move.playedBy].BallSprite);
	for (i = ROWS - 1; i >= 0; i--) {
		X = Xstart;
		for (j = 0; j < COLS; j++) {
			boardSprite.setPosition(X, Y);
			if (gameBoard[i][j] == player[0].gameBoardID) {
				(*player[0].BallSprite).setPosition(X, Y);
				window.draw(*player[0].BallSprite);
			}
			else if (gameBoard[i][j] == player[1].gameBoardID) {
				(*player[1].BallSprite).setPosition(X, Y);
				window.draw(*player[1].BallSprite);
			}
			window.draw(boardSprite);
			X += boardSprite.getLocalBounds().width;
		}
		Y += boardSprite.getLocalBounds().height;
	}

	if (move.posY >= (Ystart + ((boardSprite.getLocalBounds().height) * ((ROWS - 1) - move.row)))) {
		move.played = false;
		gameBoard[move.row][move.col] = player[move.playedBy].gameBoardID;
	}
	return move;
}

bool evaluateWinner(int** gameBoard, int ROWS, int COLS, Player player[], Move move, bool AI) {

	/*
	 gameBoard ROWS = 6 COLS = 7
	(0 , 0) (0 , 1) (0 , 2) (0 , 3) (0 , 4) (0 , 5) (0 , 6)
	(1 , 0) (1 , 1) (1 , 2) (1 , 3) (1 , 4) (1 , 5) (1 , 6)
	(2 , 0) (2 , 1) (2 , 2) (2 , 3) (2 , 4) (2 , 5) (2 , 6)
	(3 , 0) (3 , 1) (3 , 2) (3 , 3) (3 , 4) (3 , 5) (3 , 6)
	(4 , 0) (4 , 1) (4 , 2) (4 , 3) (4 , 4) (4 , 5) (4 , 6)
	(5 , 0) (5 , 1) (5 , 2) (5 , 3) (5 , 4) (5 , 5) (5 , 6)
	*/

	int minRow = -1, minCol = -1;
	int maxRow = -1, maxCol = -1;
	if (!AI) {
		if (!player[0].Active) {
			// Horizontal Check
			minRow = move.row;
			maxRow = move.row + 1;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;

			for (int i = minRow; i < maxRow; i++) {
				for (int j = minCol; j < maxCol - 3; j++) {
					if (gameBoard[i][j] == player[0].gameBoardID &&
						gameBoard[i][j + 1] == player[0].gameBoardID &&
						gameBoard[i][j + 2] == player[0].gameBoardID &&
						gameBoard[i][j + 3] == player[0].gameBoardID) {
						player[0].isWinner = 1;
						(player[0].winPos)[std::make_pair(i, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i, j + 1)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i, j + 2)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i, j + 3)] = player[0].gameBoardID;
						return 1;
					}
				}
			}
			// Vertical Check
			minCol = move.col;
			maxCol = move.col + 1;

			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;
			for (int j = minCol; j < maxCol; j++) {
				for (int i = minRow; i < maxRow - 3; i++) {
					if (gameBoard[i][j] == player[0].gameBoardID &&
						gameBoard[i + 1][j] == player[0].gameBoardID &&
						gameBoard[i + 2][j] == player[0].gameBoardID &&
						gameBoard[i + 3][j] == player[0].gameBoardID) {
						player[0].isWinner = 1;
						(player[0].winPos)[std::make_pair(i, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 1, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 2, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 3, j)] = player[0].gameBoardID;
						return 1;
					}
				}
			}
			// Diagonal Check (\)
			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;
			for (int i = maxRow - 1; i >= minRow + 3; i--) {
				for (int j = maxCol - 1; j >= minCol + 3; j--) {
					if (gameBoard[i][j] == player[0].gameBoardID &&
						gameBoard[i - 1][j - 1] == player[0].gameBoardID &&
						gameBoard[i - 2][j - 2] == player[0].gameBoardID &&
						gameBoard[i - 3][j - 3] == player[0].gameBoardID) {
						player[0].isWinner = 1;
						(player[0].winPos)[std::make_pair(i, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i - 1, j - 1)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i - 2, j - 2)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i - 3, j - 3)] = player[0].gameBoardID;
						return 1;
					}
				}
			}
			// Diagonal Check (/)
			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;
			for (int i = minRow; i < maxRow - 3; i++) {
				for (int j = maxCol - 1; j >= minCol + 3; j--) {
					if (gameBoard[i][j] == player[0].gameBoardID &&
						gameBoard[i + 1][j - 1] == player[0].gameBoardID &&
						gameBoard[i + 2][j - 2] == player[0].gameBoardID &&
						gameBoard[i + 3][j - 3] == player[0].gameBoardID) {
						player[0].isWinner = 1;
						(player[0].winPos)[std::make_pair(i, j)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 1, j - 1)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 2, j - 2)] = player[0].gameBoardID;
						(player[0].winPos)[std::make_pair(i + 3, j - 3)] = player[0].gameBoardID;
						return 1;
					}
				}
			}
		}
		else if (!player[1].Active) {
			// Horizontal Check
			minRow = move.row;
			maxRow = move.row + 1;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;

			for (int i = minRow; i < maxRow; i++) {
				for (int j = minCol; j < maxCol - 3; j++) {
					if (gameBoard[i][j] == player[1].gameBoardID &&
						gameBoard[i][j + 1] == player[1].gameBoardID &&
						gameBoard[i][j + 2] == player[1].gameBoardID &&
						gameBoard[i][j + 3] == player[1].gameBoardID) {
						player[1].isWinner = 1;
						(player[1].winPos)[std::make_pair(i, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i, j + 1)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i, j + 2)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i, j + 3)] = player[1].gameBoardID;
						return 1;
					}
				}
			}
			// Vertical Check
			minCol = move.col;
			maxCol = move.col + 1;

			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;
			for (int j = minCol; j < maxCol; j++) {
				for (int i = minRow; i < maxRow - 3; i++) {
					if (gameBoard[i][j] == player[1].gameBoardID &&
						gameBoard[i + 1][j] == player[1].gameBoardID &&
						gameBoard[i + 2][j] == player[1].gameBoardID &&
						gameBoard[i + 3][j] == player[1].gameBoardID) {
						player[1].isWinner = 1;
						(player[1].winPos)[std::make_pair(i, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 1, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 2, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 3, j)] = player[1].gameBoardID;
						return 1;
					}
				}
			}
			// Diagonal Check (\)
			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;
			for (int i = maxRow - 1; i >= minRow + 3; i--) {
				for (int j = maxCol - 1; j >= minCol + 3; j--) {
					if (gameBoard[i][j] == player[1].gameBoardID &&
						gameBoard[i - 1][j - 1] == player[1].gameBoardID &&
						gameBoard[i - 2][j - 2] == player[1].gameBoardID &&
						gameBoard[i - 3][j - 3] == player[1].gameBoardID) {
						player[1].isWinner = 1;
						(player[1].winPos)[std::make_pair(i, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i - 1, j - 1)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i - 2, j - 2)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i - 3, j - 3)] = player[1].gameBoardID;
						return 1;
					}
				}
			}
			// Diagonal Check (/)
			if (move.row + 3 >= ROWS)
				maxRow = ROWS;
			else
				maxRow = move.row + 4;
			if (move.row - 3 < 0)
				minRow = 0;
			else
				minRow = move.row - 3;

			if (move.col + 3 >= COLS)
				maxCol = COLS;
			else
				maxCol = move.col + 4;
			if (move.col - 3 < 0)
				minCol = 0;
			else
				minCol = move.col - 3;
			for (int i = minRow; i < maxRow - 3; i++) {
				for (int j = maxCol - 1; j >= minCol + 3; j--) {
					if (gameBoard[i][j] == player[1].gameBoardID &&
						gameBoard[i + 1][j - 1] == player[1].gameBoardID &&
						gameBoard[i + 2][j - 2] == player[1].gameBoardID &&
						gameBoard[i + 3][j - 3] == player[1].gameBoardID) {
						player[1].isWinner = 1;
						(player[1].winPos)[std::make_pair(i, j)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 1, j - 1)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 2, j - 2)] = player[1].gameBoardID;
						(player[1].winPos)[std::make_pair(i + 3, j - 3)] = player[1].gameBoardID;
						return 1;
					}
				}
			}
		}
	}
	else {

	}
	return 0;
}

bool drawAlert(std::string alertText, sf::Vector2i alertWindowPos) {
	//alertWindowPos.
	sf::RenderWindow windowAlert(sf::VideoMode(400, 200), "Alert", sf::Style::Close);
	windowAlert.setPosition(alertWindowPos);
	sf::Font font;
	if (!font.loadFromFile("fonts/mvboli.ttf"))
	{
		//error
	}
	sf::Texture texture;
	if (!texture.loadFromFile("images/alertBackground.jpg")) {
		//error
	}
	sf::RectangleShape rectOfPhoto;
	rectOfPhoto.setTexture(&texture);
	rectOfPhoto.setSize(sf::Vector2f(400.0, 200.0));
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(55);
	text.setFillColor(sf::Color::White);
	text.setPosition(128.0, 63.0);
	text.setString(alertText);

	while (windowAlert.isOpen()) {
		sf::Event event;
		while (windowAlert.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				windowAlert.close();
				break;
			}
		}
		windowAlert.clear();
		windowAlert.draw(rectOfPhoto);
		windowAlert.draw(text);
		windowAlert.display();
	}
	return 1;
}