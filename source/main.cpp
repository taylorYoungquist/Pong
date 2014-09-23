#include "AIE.h"
#include <iostream>
#include <fstream>

using namespace std;
//initialize constants
const int screenWidth = 780;
const int screenHeight = 672;
const char* gameTitle = "Pong";

const int minScoreToWin = 10;


enum GAMESTATES
{
	MAINMENU,
	HOWTOPLAY,
	GAMEPLAY,
	HIGHSCORES,
	PLAYER1WIN,
	PLAYER2WIN,
	END
};
//structs

struct Paddle
{
	unsigned int spriteID;
	float width;
	float height;
	float x;
	float y;
	float speed = 500.f;
	unsigned int moveUpKey;
	unsigned int moveDownKey;
	int heightExt;
	int heightLow;
	unsigned int score = 0;
	void SetSize(float a_width, float a_height)
	{
		width = a_width;
		height = a_height;
	}

	void SetPosition(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}

	void SetMovementKeys(unsigned int a_moveUp, unsigned int a_moveDown)
	{
		moveUpKey = a_moveUp;
		moveDownKey = a_moveDown;
	}

	void Move(float a_deltaTime)
	{
		if (IsKeyDown(moveUpKey))
		{
			y += speed * a_deltaTime;
			if (y + height * .5f >= heightExt)
			{
				y = heightExt - height * .5f;
			}
		}
		if (IsKeyDown(moveDownKey))
		{
			y -= speed * a_deltaTime;
			if (y - height *.5f <= heightLow)
			{
				y = heightLow + height*.5f;
			}
		}
		MoveSprite(spriteID, x, y);
	}
	void AddScore(unsigned int a_score)
	{
		score += a_score;
	}
	void GetScore(char* a_result)
	{
		itoa(score, a_result, 10);
	}
};
Paddle paddle1;
Paddle paddle2;
struct Ball
{
	unsigned int ballID;
	float width;
	float height;
	float x;
	float y;
	int heightExt;
	int heightLow;
	float speedX;
	float speedY;
	void SetSize(float a_width, float a_height)
	{
		width = a_width;
		height = a_height;
	}

	void SetPosition(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}
	void Move(float a_deltaTime)
	{
		x -= speedX * a_deltaTime;
		y -= speedY * a_deltaTime;
		if (y + height * .5f >= heightExt)
		{
			y = heightExt - height * .5f;
			speedY *= -1;
		}
		if (y - height * .5f <= heightLow)
		{
			y = heightLow + height * .5f;
			speedY *= -1;
		}
		if (x + width * .5f >= screenWidth)
		{
			x = screenWidth *.5f;
			paddle1.AddScore(1);
		}
		if (x - width * .5f <= 0)
		{
			x = screenWidth *.5f;
			paddle2.AddScore(1);
		}
	}
	bool paddle1Collision()
	{
		if (x - width * .5f <= paddle1.x + paddle1.width *.5f &&
			x - width * .5f >= paddle1.x - paddle1.width *.5f &&
			y - height <= paddle1.y + paddle1.height *.5f &&
			y + height >= paddle1.y - paddle1.height *.5f)
		{
			while (x >= paddle1.x)
			{
				return true;
			}
			return false;
		}
	}
	bool paddle2Collision()
	{
		if (x + width * .5f >= paddle2.x - paddle2.width *.5f &&
			x - width * .5f <= paddle2.x + paddle2.width *.5f &&
			y - height >= paddle2.y - paddle2.height *.5f &&
			y + height <= paddle2.y + paddle2.height *.5f)
		{
			while (x <= paddle2.x)
			{
				return true;
			}
		}
		return false;
	}
	void Update(float a_deltaTime)
	{
		if (x <= 0)
		{
			paddle2.AddScore(1);
		}
		else
		{
			if (x >= screenWidth)
			{
				paddle1.AddScore(1);
			}
		}
	}
};
Ball ball;



int mHighScore;
GAMESTATES currentState = MAINMENU;
void UpdateMainMenu();
void UpdateGamePlay();
void UpdateHowToPlay();
void GameUI();
void WriteHighScore();
void LoadHighScore();

int main(int argc, char* argv[])
{
	bool runGame = true;
	Initialise(screenWidth, screenHeight, false, gameTitle);
	//fstream file;

	//Player one drawing/creating
	paddle1.heightExt = screenHeight;
	paddle1.SetPosition(50.f, screenHeight *.5f);
	paddle1.SetSize(10.f, 120.f);
	paddle1.SetMovementKeys('W', 'S');
	paddle1.spriteID = CreateSprite("./images/PlayerPaddle.png", paddle1.width, paddle1.height, true);
	MoveSprite(paddle1.spriteID, paddle1.x, paddle1.y);
	//player two
	paddle2.heightExt = screenHeight;
	paddle2.SetPosition(screenWidth - 50.f, screenHeight * .5f);
	paddle2.SetSize(10.f, 120.f);
	paddle2.SetMovementKeys('I', 'K');
	paddle2.spriteID = CreateSprite("./images/PlayerPaddle2.png", paddle2.width, paddle2.height, true);
	MoveSprite(paddle2.spriteID, paddle2.x, paddle2.y);
	// background
	SetBackgroundColour(SColour(0, 0, 0, 255));

	//Ball
	ball.speedX = 300.f;
	ball.speedY = 300.f;
	ball.heightExt = screenHeight;
	ball.SetPosition(screenWidth *.5f, screenHeight * .5f);
	ball.SetSize(20.f, 20.f);
	ball.ballID = CreateSprite("./images/Ball.png", ball.width, ball.height, true);
	MoveSprite(ball.ballID, ball.x, ball.y);



	do
	{

		ClearScreen();
		float deltaTime = GetDeltaTime();
		//Change State
		switch (currentState)
		{
		case MAINMENU:
			UpdateMainMenu();
			if (IsKeyDown(256))
			{
				runGame = false;
			}
			break;
		case HOWTOPLAY:
			UpdateHowToPlay();
			if (IsKeyDown(256))
			{
				runGame = false;
			}
			break;
		case GAMEPLAY:
			UpdateGamePlay();
			if (IsKeyDown(256))
			{
				runGame = false;
			}
			if (paddle1.score == 10)
			{
				currentState = PLAYER1WIN;
			}
			if (paddle2.score == 10)
			{
				currentState = PLAYER2WIN;
			}
			break;
		case HIGHSCORES:
			DrawString("HIGH SCORE", screenWidth * .5f - 100.f, screenHeight *0.75f);
			char buff[30];
			DrawString(itoa(mHighScore, buff, 10), screenWidth *.5f - 25, screenHeight *.66f);
			DrawString("Q to main menu", screenWidth *.375f, 50);
			if (IsKeyDown('Q'))
			{
				currentState = MAINMENU;
			}
			break;
		case PLAYER1WIN:
			DrawString("Player One Has Won The Game! Press Q To Go To Main Menu", 10, screenHeight *.5f);
			mHighScore = paddle1.score;
			if (IsKeyDown('Q'))
			{
				currentState = MAINMENU;
			}
			break;
		case PLAYER2WIN:
			DrawString("Player Two Has Won The Game! Press Q To Go TO Main Menu", 10, screenHeight * .5f);
			mHighScore = paddle2.score;
			if (IsKeyDown('Q'))
			{
				currentState = MAINMENU;
			}
			break;
		case END:
			if (IsKeyDown(256))
			{
				runGame = false;
			}
			break;
		default:
			break;
		}

	} while (!FrameworkUpdate() && runGame);

	Shutdown();

	return 0;
}

void UpdateMainMenu()
{
	//Giving the options
	DrawString("Play(P)", screenWidth *.375f, 500);
	DrawString("How to Play(Enter)", screenWidth *.375f, 400);
	DrawString("High Scores(H)", screenWidth * .375f, 300);
	DrawString("Quit(Esc)", screenWidth * .375f, 200);
	if (IsKeyDown('P'))
	{
		currentState = GAMEPLAY;
	}
	if (IsKeyDown('H'))
	{
		currentState = HIGHSCORES;
	}
	if (IsKeyDown(257))
	{
		currentState = HOWTOPLAY;
	}
}

void UpdateHowToPlay()
{
	//Need to Draw all the instructions.
	DrawString("HOW TO PLAY", screenWidth * .375f, 650);
	DrawString("The objective of Pong is to get the ball past the enemy player", 25, 600);
	DrawString("while also preventing them from getting the ball past your", 25, 550);
	DrawString("paddle. The first player to reach a score of ten will win!", 25, 500);
	DrawString("CONTROLS", screenWidth * .375f, 400);
	DrawString("Player 1:", 150, 350);
	DrawString("Up: W", 150, 300);
	DrawString("Down: S", 150, 250);
	DrawString("Player 2:", 475, 350);
	DrawString("Up: I", 475, 300);
	DrawString("Down: K", 475, 250);
	DrawString("GOOD LUCK", screenWidth * .375f, 150);
	DrawString("Press Q to go back....", screenWidth * .33f, 50);
	//Need to return to the main menu from here to be able to access the game
	if (IsKeyDown('Q'))
	{
		currentState = MAINMENU;
	}
}

void UpdateGamePlay()
{
	float delta = GetDeltaTime();
	//Player1
	paddle1.Move(delta);
	MoveSprite(paddle1.spriteID, paddle1.x, paddle1.y);
	DrawSprite(paddle1.spriteID);
	//Player2
	paddle2.Move(delta);
	MoveSprite(paddle2.spriteID, paddle2.x, paddle2.y);
	DrawSprite(paddle2.spriteID);

	//Ball
	ball.Move(delta);
	DrawSprite(ball.ballID);
	MoveSprite(ball.ballID, ball.x, ball.y);

	if (ball.paddle1Collision() == true)
	{
		ball.speedX *= -1;
	}
	if (ball.paddle2Collision() == true)
	{
		ball.speedX *= -1;
	}

	void LoadHighScore();
	void WriteHighScore();
	GameUI();

}
void GameUI()
{
	char paddle1Score[3];
	char paddle2Score[3];
	paddle1.GetScore(paddle1Score);
	paddle2.GetScore(paddle2Score);
	DrawString(paddle1Score, 30, screenHeight - 20);
	DrawString(paddle2Score, screenWidth - 30, screenHeight - 20);

}
void LoadHighScore()
{
	fstream file;
	file.open("file.txt", ios::in);
	char buffer[10];
	file.getline(buffer, 10);
	mHighScore = atoi(buffer);
	file.close();
}

void WriteHighScore()
{
	fstream file;
	file.open("file.txt", ios::out);
	file << mHighScore;
	file.close();
}