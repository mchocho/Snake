#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <Windows.h>
//#include <conio.h> //console input output FACEPALM

const int screenWidth = 80;
const int screenHeight = 30;

const int WIDTH = 20;
const int HEIGHT = 20;
const bool BOUNDARIES = true; //Player can't leave the game arena
bool bGameOver;
int x, y, nFruitX, nFruitY, nScore;
int nTailX[100], nTailY[100];	//
int nTail;			//Specifies the length of the tail

unsigned char *pField = nullptr;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void resetFruit() {
	//Place fruit at random position
	nFruitX = rand() % WIDTH;
	nFruitY = rand() % HEIGHT;
}

void Setup()
{
	bGameOver = false;
	dir = STOP;

	//Center snake
	x = WIDTH / 2;
	y = HEIGHT / 2;

	resetFruit();
	nScore = 0;
}

void Draw()
{
	//Let's draw without calling the system
	 wchar_t *screen = new wchar_t[screenWidth * screenHeight];
	//system("clear"); //For linux
	//system("cls");	//windows
	 pField = new unsigned char[WIDTH * HEIGHT];


	//Print the top wall
	 for (int i = 0; i < screenWidth * screenHeight; i++)
		 screen[i] = L'X';

	//Print the top wall
	for (int i = 0; i < WIDTH + 2; i++)
		std::cout << "#";

	for (int i = 0; i < HEIGHT; i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == 0)
				std::cout << "#"; //Print left wall
			if (i == y && j == x)
				std::cout << "O"; //Print the snake head (player)
			else if (i == nFruitY && j == nFruitX)
				std::cout << "F";	//Print the fruit
			else
			{
				bool print = false; //Keeps track of whether we printed the tail segment
				//Loop through every element of the tail and draw it
				for (int k = 0; k < nTail; k++)
				{
					//std::cout << "Tail size " << nTail << std::endl;
					//If tailX is our current x coord and tailY is our current y coord
					if (nTailX[k] == j && nTailY[k] == i)
					{
						print = true;
						std::cout << "o";
					}
				}
				if (!print)
					std::cout << " ";
			}

			if (j == WIDTH - 1)
				std::cout << "#"; //Print right wall
		}
	}

	std::cout << std::endl;
	//Print bottom wall
	for (int i = 0; i < WIDTH + 2; i++)
		std::cout << "#";
	std::cout << std::endl << std::endl << "Score: " << nScore << std::endl;
}

void Input() //Controller
{
	//w - up
	//a - left
	//d - right
	//s - down

	if (GetKeyState('A') & 0x8000)
		dir = LEFT;
	else if (GetKeyState('D') & 0x8000)
		dir = RIGHT;
	else if (GetKeyState('W') & 0x8000)
		dir = UP;
	else if (GetKeyState('S') & 0x8000)
		dir = DOWN;
}

void Logic() //Player
{
	int nPrevX = nTailX[0];
	int nPrevY = nTailY[0];
	int nPrev2X, nPrev2Y;

	nTailX[0] = x;
	nTailY[0] = y;
	//i = 1 because the head already exists
	for (int i = 1; i < nTail; i++)
	{
		//We're breaking the snake into segments
		nPrev2X = nTailX[i];
		nPrev2Y = nTailY[i];
		nTailX[i] = nPrevX;
		nTailY[i] = nPrevY;
		nPrevX = nPrev2X;
		nPrevY = nPrev2Y;
	}

	switch(dir)
	{
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		default:
			break;
	}

	if (BOUNDARIES)
	{
		if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1)//Out of game bounds
			bGameOver = true;
	}
	else {
		//This allows us to jump through walls
		if (x >= WIDTH)
			x = 0;
		else if (x < 0)
			x = WIDTH - 1;
	
		if (y >= HEIGHT)
			y = 0;
		else if (y < 0)
			y = HEIGHT - 1;
	}
	
	for (int i = 0; i < nTail; i++)
		if (nTailX[i] == x && nTailY[i] == y)
			bGameOver = true;
	if (x == nFruitX && y == nFruitY) //We ate the fruit
	{
		nScore += 10; 
		resetFruit();
		
		//Increase tail size
		nTail++;
	}
}

int main()
{
	Setup();

	while (!bGameOver)
	{
		Draw();
		Input();
		Logic();
		//Sleep(10);	//Sleep for 10 milliseconds
	}	
	return 0;
}
