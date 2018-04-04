#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <iomanip>

using namespace std;

int point_x = 0, point_y = 0, bonus_x = 0, bonus_y = 0, snake_x = 32, snake_y = 16;
int snake_l = 3, score = 0, move_1 = 0, bonus = 0, up_speed = 0, speed = 200;
const int height = 30, width = 60;
int snake[500][2];
bool game_start = 0;

enum COLORS {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	AQUA = 3,
	RED = 4,
	PURPLE = 5,
	YELLOW = 6,
	WHITE = 7,
	GRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_AQUA = 11,
	LIGHT_RED = 12,
	LIGHT_PURPLE = 13,
	LIGHT_YELLOW = 14,
	BRIGHT_WHITE = 15
};

enum CORNERS {
	TOP_LEFT = 201,
	TOP_RIGHT = 187,
	BOTTOM_LEFT = 200,
	BOTTOM_RIGHT = 188,
	CENTER_LEFT = 186,
	CENTER_RIGHT = CENTER_LEFT,
	MIDDLE = 205
};

enum MOVE {
	UP = 72,
	DOWN = 80,
	RIGHT = 77,
	LEFT = 75,
	NOTHING = 0
};

void my_set_color(int fg, int bg)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, fg | bg << 4);
}

void my_set_cursor(int x, int y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	COORD coord = {};
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(h, coord);
	SetConsoleCursorInfo(h, &info);
}

void game_score()
{
	my_set_color(BLACK, GRAY);
	my_set_cursor(2, 1);
	cout << " SCORE : " << score << " ";
	my_set_color(WHITE, BLACK);
}

void game_timer(int time)
{
	if (time >= 3 && time <= 5) my_set_color(WHITE, GREEN);
	else my_set_color(WHITE, RED);
	my_set_cursor(47, 1);
	cout << " TIME : 00:0" << time << " ";
	my_set_color(WHITE, BLACK);
	if (time == 0) { my_set_cursor(47, 1); cout << "              "; }
}

void speed_up(bool a)
{
	if (a == 0) {
		my_set_color(WHITE, PURPLE);
		my_set_cursor(28, 1);
		cout << "  SPEED UP!  ";
		my_set_color(WHITE, BLACK);
	}
	else {
		my_set_color(BLACK, BLACK);
		my_set_cursor(28, 1);
		cout << "  SPEED UP!  ";
		my_set_color(WHITE, BLACK);
	}
}

void draw_area()
{
	for (int i = 0; i <= width; ++i)
	{
		my_set_cursor(i + 1, 2);
		cout << (char)MIDDLE;
		my_set_cursor(i + 1, height + 2);
		cout << (char)MIDDLE;
	}

	for (int i = 0; i <= height; ++i)
	{
		if (i == 0)
		{
			my_set_cursor(1, i + 2);
			cout << (char)TOP_LEFT;
			my_set_cursor(width + 1, i + 2);
			cout << (char)TOP_RIGHT;
		}
		else if (i == height)
		{
			my_set_cursor(1, i + 2);
			cout << (char)BOTTOM_LEFT;
			my_set_cursor(width + 1, i + 2);
			cout << (char)BOTTOM_RIGHT;
		}
		else
		{
			my_set_cursor(1, i + 2);
			cout << (char)CENTER_LEFT;
			my_set_cursor(width + 1, i + 2);
			cout << (char)CENTER_RIGHT;
		}
	}
	my_set_cursor(1, height + 3);
}


void game_intro()
{
	my_set_color(LIGHT_BLUE, BLACK);
	draw_area();
	my_set_cursor(width / 2 - 9, height / 4);
	my_set_color(BLACK, LIGHT_BLUE);
	cout << "      WELCOME !      ";
	my_set_cursor(width / 2 - 17, height / 2 + 2);
	my_set_color(WHITE, BLACK);
	cout << "->> TAPE ENTER TO START THE GAME <<-";
	my_set_cursor(width / 2 - 11, height + 2);
	my_set_color(BLACK, LIGHT_BLUE);
	cout << "  Fuad Agazade  |  2017  ";
	my_set_color(BLACK, BLACK);
	my_set_cursor(1, height + 3);
	system("pause");
	my_set_color(WHITE, BLACK);
}

void game_over()
{
	my_set_color(RED, BLACK);
	draw_area();
	my_set_cursor(width / 2 - 9, height / 2);
	my_set_color(BLACK, RED);
	cout << "     GAME OVER !     ";
	my_set_cursor(width / 2 - 9, height / 2 + 2);
	my_set_color(WHITE, RED);
	cout << "     SCORE : " << setw(3) << score << "     ";

	my_set_cursor(1, height);
	my_set_color(BLACK, YELLOW);
	cout << " TAPE 'ESC' IF YOU WANT QUIT GAME OR 'ENTER' TO RESTART GAME " << endl;
	my_set_color(WHITE, BLACK);
	int a = _getch();
	if (a == 13) system("cls");
	else game_start = 1;
}

void game_point()
{
	bool free = 0;

	while (free == 0)
	{
		point_x = rand() % (width - 1) + 2;
		point_x % 2 == 0 ? point_x += 0 : point_x += 1;
		point_y = rand() % (height - 1) + 3;
		for (int i = 1; i < snake_l; i++) {
			if ((point_x == snake[i][0] && point_y == snake[i][1]) || (point_y == bonus_y && point_x == bonus_x)) { free = 0; break; }
			else free = 1;
		}
	}

	my_set_cursor(point_x, point_y);
	my_set_color(LIGHT_GREEN, BLACK);
	cout << "o";
	my_set_color(WHITE, BLACK);
}

void game_bonus()
{
	bool free = 0;

	while (free == 0)
	{
		bonus_x = rand() % (width - 1) + 2;
		bonus_x % 2 == 0 ? bonus_x += 0 : bonus_x += 1;
		bonus_y = rand() % (height - 1) + 3;
		for (int i = 1; i < snake_l; i++) {
			if ((bonus_x == snake[i][0] && bonus_y == snake[i][1]) || (bonus_y == point_y && bonus_x == point_x)) { free = 0; break; }
			else free = 1;
		}
	}

	my_set_cursor(bonus_x, bonus_y);
	my_set_color(LIGHT_RED, BLACK);
	cout << "o";
	my_set_color(WHITE, BLACK);
}

void game_control(int move)
{
	switch (move)
	{
	case UP:
		--snake[0][1];
		break;
	case DOWN:
		++snake[0][1];
		break;
	case RIGHT:
		snake[0][0] += 2;
		break;
	case LEFT:
		snake[0][0] -= 2;
		break;
	default:
		break;
	}
}

void game_snake()
{
	int game = 0;
	int timer = 0;
	int s_time = 5;
	int move = RIGHT;

	for (int i = 0; i < snake_l; ++i)
	{
		snake[i][0] = snake_x - i;
		snake[i][1] = snake_y;
	}

	while (game == 0)
	{
		game_score();
		Sleep(speed);
		if (_kbhit())
		{
			_getch();
			move_1 = _getch();
		}
		if ((move_1 == UP && move != DOWN) || (move_1 == DOWN && move != UP) || (move_1 == LEFT && move != RIGHT) || (move_1 == RIGHT && move != LEFT))
		{
			move = move_1;
		}
		game_control(move);
		my_set_cursor(snake[0][0], snake[0][1]);
		cout << "O";
		for (int i = 1; i < snake_l; ++i)
		{
			my_set_cursor(snake[i][0], snake[i][1]);
			cout << 'o';
		}

		my_set_cursor(snake[snake_l][0], snake[snake_l][1]);
		cout << " ";

		for (int i = snake_l; i >= 1; --i)
		{
			snake[i][0] = snake[i - 1][0];
			snake[i][1] = snake[i - 1][1];
		}

		if (snake[0][0] == point_x && snake[0][1] == point_y)
		{
			my_set_cursor(point_x, point_y);
			cout << "\b\a";
			++snake_l;
			++bonus;
			score += 10;
			game_point();

			if (bonus == 5) {
				s_time = 5;
				timer = 0;
				game_bonus();
				game_timer(5);
				if (speed != 40) { speed -= 20; speed_up(0); }
			}
		}

		if (bonus >= 5)
		{
			++timer;
			if (s_time == 0)
			{
				my_set_cursor(bonus_x, bonus_y);
				cout << " ";
				bonus = 0;
			}
			else if (timer % 8 == 0)
			{
				--s_time;
				game_timer(s_time);
				speed_up(1);
			}
		}

		if (snake[0][0] == bonus_x && snake[0][1] == bonus_y && s_time != 0)
		{
			my_set_cursor(bonus_x, bonus_y);
			cout << "\b\a";
			snake_l += 5;
			score += 50;
			bonus = 0;
			game_timer(0);
		}

		for (int i = 3; i < snake_l; i++) {
			if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1])
				game = 1;
		}

		if (snake[0][0] <= 1 || snake[0][0] == width + 2 || snake[0][1] == 2 || snake[0][1] == height + 2)
			game = 1;
	}
}

void main()
{
	system("MODE 63, 34");
	system("TITLE, --- Snake! ---  Fuad Agazade");
	srand(time(0));

	game_intro();
	system("cls");

	while (game_start == 0)
	{
		draw_area();
		game_point();
		game_snake();

		system("cls");
		game_over();
		my_set_cursor(1, height + 3);
		my_set_color(WHITE, BLACK);
	}
	my_set_color(BLACK, BLACK);
}
