#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>

using namespace std;

const char snake_char = 'o';
const char food_char = '@';
int food_x, food_y;
const int length_max = 12;
int length = 5;
int coord_x[length_max] = { 0 };
int coord_y[length_max] = { 0 };
int direct;
int sleeper = 200;

void hide_cursor()
{
	HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(ch, &info);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void border()
{
	for (int x = 1; x < 100; x += 2)
	{
		gotoxy(x, 1);
		printf(".");
	}
	for (int x = 1; x < 100; x += 2)
	{
		gotoxy(x, 24);
		printf(".");
	}
	for (int y = 1; y < 24; y++)
	{
		gotoxy(1, y);
		printf(".");
	}
	for (int y = 1; y < 24; y++)
	{
		gotoxy(99, y);
		printf(".");
	}
}

bool game_over()
{
	if (((coord_x[0] <= 1) ||
		(coord_x[0] >= 100) ||
		(coord_y[0] <= 1) ||
		(coord_y[0] >= 24)) || (((coord_x[0] <= coord_x[length - 1] && coord_x[0] >= coord_x[1]) && (coord_y[0] <= coord_y[length - 1] && coord_y[0] >= coord_y[1]))))
	{
		gotoxy(45, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("GAME OVER");
		return true;
	}
	return false;
}

void draw()
{
	for (int i = 0; i < length; i++)
	{
		gotoxy(coord_x[i], coord_y[i]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		printf("%c", snake_char);
	}
}

void food()
{
	bool flag = false;
	while (!flag)
	{
		food_x = rand() % 97 + 2;
		if (food_x % 2 == 1) food_x++;
		food_y = rand() % 22 + 2;
		flag = true;
		for (int i = 0; i < length; i++)
		{
			if (food_x == coord_x[i] && food_y == coord_y[i]) { flag = false; continue; }
		}
	}
	gotoxy(food_x, food_y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("%c", food_char);
}

bool eat_food()
{
	switch (direct)
	{
	case 0: if (coord_x[0] == food_x && coord_y[0] == food_y - 1)
		return true;
	case 1: if (coord_x[0] == food_x - 2 && coord_y[0] == food_y)
		return true;
	case 2: if (coord_x[0] == food_x + 2 && coord_y[0] == food_y)
		return true;
	case 3: if (coord_x[0] == food_x && coord_y[0] == food_y + 1)
		return true;
	}
	return false;
}

void step()
{
	gotoxy(coord_x[length - 1], coord_y[length - 1]);
	printf(" ");
	for (int i = length - 1; i > 0; i--)
	{
		coord_x[i] = coord_x[i - 1];
		coord_y[i] = coord_y[i - 1];
	}
	switch (direct)
	{
	case 0: coord_y[0] = coord_y[1] - 1;
		break;
	case 1: coord_x[0] = coord_x[1] + 2;
		break;
	case 2: coord_y[0] = coord_y[1] + 1;
		break;
	case 3: coord_x[0] = coord_x[1] - 2;
		break;
	}
}

void grow_snake()
{
	for (int i = length; i > 0; i--)
	{
		coord_x[i] = coord_x[i - 1];
		coord_y[i] = coord_y[i - 1];
	}
	length++;
	if (length == length_max)
	{
		gotoxy(45, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		printf("YOU WON");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		gotoxy(1, 24);
		exit(0);
	}
	coord_x[0] = food_x;
	coord_y[0] = food_y;
	sleeper -= 20;
}

int main()
{
	hide_cursor();
	srand(time(NULL));
	border();
	food();
	direct = rand() % 4;
	int x_rand = rand() % 80 + 11, y_rand = rand() % 10 + 11;
	coord_x[0] = x_rand; coord_y[0] = y_rand;
	x_rand += 2;
	coord_x[1] = x_rand; coord_y[1] = y_rand;
	x_rand += 2;
	coord_x[2] = x_rand; coord_y[2] = y_rand;
	x_rand += 2;
	coord_x[3] = x_rand; coord_y[3] = y_rand;
	x_rand += 2;
	coord_x[4] = x_rand; coord_y[4] = y_rand;
	int key;
	while (true)
	{
		draw();
		if (_kbhit())
		{
			key = _getch();
			switch (key)
			{
			case 224: continue; break;
			case 80: direct = 2; break;
			case 75: direct = 3; break;
			case 72: direct = 0; break;
			case 77: direct = 1; break;
			}
		}
		step();
		Sleep(sleeper);
		if (game_over()) break;
		if (eat_food())
		{
			grow_snake();
			food();
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	gotoxy(1, 24);
	return 0;
}