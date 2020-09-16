#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <Windows.h>

const int SCREENWIDTH = 80;
const int SCREENHEIGHT = 30;
const int SCREENAREA = SCREENWIDTH * SCREENHEIGHT;

const int WIDTH = 20;
const int HEIGHT = 20;
const int AREA = WIDTH * HEIGHT;

const bool BOUNDARIES = true; //Player can't leave the game arena
bool bGameOver;
int x, y, nFruitX, nFruitY, nScore;
int nTailX[100], nTailY[100];	//
int nTail;			//Specifies the length of the tail

wchar_t *screen = new wchar_t[AREA];

unsigned char *pField = nullptr;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void resetFruit() {
	//Place fruit at random position
	nFruitX = rand() % WIDTH;
	nFruitY = rand() % HEIGHT;

	if (nFruitX == 0)
		++nFruitX;
	else if (nFruitX == WIDTH - 1)
		--nFruitX;

	if (nFruitY == 0)
		++nFruitX;
	else if (nFruitY == HEIGHT - 1)
		--nFruitY;
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

void Draw(HANDLE hConsole)
{
	//Let's draw without calling the system
	DWORD dwBytesWritten = 0;
	
	//Print to the buffer
	 for (int i = 0; i < SCREENAREA; i++)
		screen[i] = L'#';

	//Print the top wall
	for (int i = 0; i < WIDTH + 2; i++)
		screen[i] = L'#';

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			int nPos = (i + 2) * SCREENWIDTH + (j + 2);

			if (j == 0)
				screen[nPos] = L'#'; //Print left wall

			//Snake
			if (i == y && j == x)
				screen[nPos] = L'O'; //Print the snake head (player)
			else if (i == nFruitY && j == nFruitX)
				screen[nPos] = L'F';//+1F347	//Print the fruit
			else
			{
				bool print = false; //Keeps track of whether we printed the tail segment
				//Loop through every element of the tail and draw it
				for (int k = 0; k < nTail; k++)
				{
					//If tailX is our current x coord and tailY is our current y coord
					if (nTailX[k] == j && nTailY[k] == i)
					{
						print = true;
						screen[nPos] = L'o';
					}
				}
				if (!print)
					screen[nPos] = L' ';
			}

			if (j == WIDTH - 1)
				screen[nPos] = L'#';
		}
	}

	//Print bottom wall
	for (int i = 0; i < WIDTH + 2; i++)
		screen[((HEIGHT - 1) + 2) * SCREENWIDTH + i] = L'#';
 
	//Display Frame
	WriteConsoleOutputCharacter(hConsole, screen, SCREENAREA, {0, 0}, &dwBytesWritten);
}

void Input() //Controller
{
	//w - up
	//a - left
	//d - right
	//s - down
	//escape - quit

	if (GetKeyState('A') & 0x8000)
		dir = LEFT;
	else if (GetKeyState('D') & 0x8000)
		dir = RIGHT;
	else if (GetKeyState('W') & 0x8000)
		dir = UP;
	else if (GetKeyState('S') & 0x8000)
		dir = DOWN;
	else if (GetKeyState(0x1B) & 0x8000)
		bGameOver = true;	
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
		if (x >= WIDTH - 1)
			x = 0;
		else if (x < 0)
			x = WIDTH - 1;
	
		if (y >= HEIGHT - 1)
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
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	
	Setup();
	while (!bGameOver)
	{
		Draw(hConsole);
		Input();
		Logic();
		Sleep(80);	//Sleep for 80 milliseconds
	}	
	return 0;
}
