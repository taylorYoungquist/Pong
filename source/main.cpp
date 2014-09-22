#include "AIE.h"
#include <iostream>
//initialize constants
const int screenWidth = 780;
const int screenHeight = 672;
const char* gameTitle = "Pong";


enum GAMESTATES
{
	MAINMENU,
	HOWTOPLAY,
	GAMEPLAY,
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
			if (y +height * .5f >=heightExt)
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
		}
		if (x - width * .5f <= 0)
		{
			x = screenWidth *.5f;
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
};
Ball ball;

GAMESTATES currentState = MAINMENU;
void UpdateMainMenu();
void UpdateGamePlay();
void UpdateHowToPlay();
int main( int argc, char* argv[] )
{	
    Initialise(screenWidth, screenHeight, false, gameTitle);
    
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
	ball.speedX = 100.f;
	ball.speedY = 100.f;
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
			break;
		case HOWTOPLAY:
			UpdateHowToPlay();
			break;
		case GAMEPLAY:
			UpdateGamePlay();
			break;
		case END:
			break;
		default:
			break;
		}

    } while(!FrameworkUpdate());
	
    Shutdown();

    return 0;
}

void UpdateMainMenu()
{
	//Giving the options
	DrawString("Play(P)", screenWidth *.375f, 500);
	DrawString("How to Play(H)", screenWidth *.375f, 400);
	if (IsKeyDown('P'))
	{
		currentState = GAMEPLAY;
	}
	if (IsKeyDown('H'))
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
	DrawSprite(paddle1.spriteID);
	MoveSprite(paddle1.spriteID, paddle1.x, paddle1.y);
	//Player2
	paddle2.Move(delta); 
	DrawSprite(paddle2.spriteID);
	MoveSprite(paddle2.spriteID, paddle2.x, paddle2.y);

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
	
}
