#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <ctime>

using namespace std;

const int ROWS = 44, COLUMNS = 119;
char Matrix[ROWS][COLUMNS];

class Snake_body
{
public:
	int _x, _y;

	Snake_body(int x = 0, int y = 0)
	{
		_x = x; _y = y;
	}
};

enum Direction
{
	move_left = 1,
	move_right,
	move_up,
	move_down
};

//LPTSTR ReadConsoleOut(int x, int y)//Возвращает * на символ в заданных координатах консоли
//{
//	COORD pos;
//	pos.X = x; pos.Y = y;
//	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
//	LPTSTR lpCharacter = new TCHAR[1];
//	DWORD dwReaden = 0;
//	ReadConsoleOutputCharacter(hCon, lpCharacter, 1, pos, &dwReaden);
//	return lpCharacter;
//}

void SetPos(int x, int y)
{
	COORD pos;
	pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int Keyboard(int& move)
{
	bool left_arrow = GetKeyState(0x25) & 0x8000;//движение влево
	bool up_arrow = GetKeyState(0x26) & 0x8000;//движение вверх
	bool right_arrow = GetKeyState(0x27) & 0x8000;//движение вправо
	bool down_arrow = GetKeyState(0x28) & 0x8000;//движение вниз

	bool esc = GetKeyState(0x1B) & 0x8000;//выход из игры

	if (esc)
	{
		exit(0);
	}
	if (left_arrow)
	{
		move = move_left;
	}
	if (up_arrow)
	{
		move = move_up;
	}
	if (right_arrow)
	{
		move = move_right;
	}
	if (down_arrow)
	{
		move = move_down;
	}

	return move;
};

void Snake(Snake_body& snake, char body = 'O')
{
	Matrix[snake._y][snake._x] = body;
	SetPos(snake._x, snake._y);
	cout << body;
}

void SnakeMove(vector<Snake_body>& snake, int step_x = 0, int step_y = 0)
{
	int x = snake[0]._x, y = snake[0]._y;
	int temp_x, temp_y;

	snake[0]._x += step_x; snake[0]._y += step_y;
	if (Matrix[snake[0]._y][snake[0]._x] == 'O')
	{
		exit(0);
	}
	Snake(snake[0], '@');

	for (size_t i = 1; i < snake.size(); i++)
	{
		temp_x = snake[i]._x, temp_y = snake[i]._y;
		snake[i]._x = x; snake[i]._y = y;
		x = temp_x; y = temp_y;
		Snake(snake[i]);
	}
	Matrix[y][x] = ' ';
	SetPos(x, y);
	cout << ' ';
}

void Rabbit(int x, int y, char rabbit)
{
	Matrix[y][x] = rabbit;
	SetPos(x, y);
	cout << rabbit;
}

int Borders(int& x, int& y)
{
	if (x > 118)
	{
		SetPos(x, y);
		cout << ' ';
		x = 1;
	}
	if (x < 1)
	{
		SetPos(x, y);
		cout << ' ';
		x = 118;
	}
	if (y > 43)
	{
		SetPos(x, y);
		cout << ' ';
		y = 1;
	}
	if (y < 1)
	{
		SetPos(x, y);
		cout << ' ';
		y = 43;
	}
	return x, y;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	SetWindowPos(GetConsoleWindow(), NULL, 0, 0, 1220, 780, NULL);
	srand(time(NULL));
	system("color A0");

	int snake_x = 60; int snake_y = 23;
	int rabbit_x, rabbit_y;
	bool notEaten = false;
	int move = 3;

	Snake_body snake_body(snake_x, snake_y);
	vector<Snake_body> snake;
	snake.push_back(snake_body);
	snake.push_back(snake_body);

	for (size_t i = 0; i <= ROWS; i++)
	{
		for (size_t j = 0; j <= COLUMNS; j++)
		{
			Matrix[i][j] = ' ';
		}
	}
	
	while (true)
	{
		if (!notEaten)
		{
			do
			{
				rabbit_x = rand() % 118+1; rabbit_y = rand() % 43+1;

			} while (Matrix[rabbit_y][rabbit_x] != ' ');
			notEaten = true;
		}
		Rabbit(rabbit_x, rabbit_y, '&');

		switch (Keyboard(move))
		{
		case move_left:
			Borders(snake[0]._x, snake[0]._y);
			SnakeMove(snake, -1, 0);
			break;
		case move_up:
			Borders(snake[0]._x, snake[0]._y);
			SnakeMove(snake, 0, -1);
			break;
		case move_right:
			Borders(snake[0]._x, snake[0]._y);
			SnakeMove(snake, 1, 0);
			break;
		case move_down:
			Borders(snake[0]._x, snake[0]._y);
			SnakeMove(snake, 0, 1);
			break;
		}
		if (snake[0]._x == rabbit_x && snake[0]._y == rabbit_y)
		{
			Rabbit(rabbit_x, rabbit_y, ' ');
			notEaten = false;
			snake_body._x = snake[snake.size() - 1]._x;
			snake_body._y = snake[snake.size() - 1]._y;
			snake.push_back(snake_body);
		}

		this_thread::sleep_for(0.1s);
	}

	system("color 02");
	return 0;
};
