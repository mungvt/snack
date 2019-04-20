
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include "console.h"
#include <iostream>
#include <fstream>
#include <math.h>

#define consoleWidth	85
#define consoleHeight	30

enum TrangThai { UP, DOWN, LEFT, RIGHT };
bool pastPress[4];										//cap nhap trang thai trong qua khu

int map[consoleHeight][consoleWidth];					//Cap nhap map
int chooseMap;											//Chon map

struct ToaDo
{
	int y, x;											// y là tung độ, x là hoành độ
};

struct HoaQua
{
	ToaDo td;
	ToaDo tim;
};


struct Snake
{
	ToaDo dot[100];
	int n;
	TrangThai tt;
};

void Nocursortype()										// Xóa con trỏ chuột trên màn hình console
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

int ChonMap() {
	gotoXY(30, 10);
	printf("==>Chon Map<==\n");
	printf("1. No Wall\n");
	printf("2. EEG Map\n");
	printf("3. Never Die\n");
	printf("4. Snake Bot\n");
	printf("5. Exit\n");
	printf("Your choose: ");
	int i = 0;
	scanf("%d", &i);
	return i;
}

void KhoiTao(Snake& snake, HoaQua& hq)
{
	snake.n = 1;
	snake.dot[0].x = 0;
	snake.dot[0].y = 0;

	snake.tt = RIGHT;

	hq.td.x = 20;
	hq.td.y = 1;
	hq.tim.x = 25;
	hq.tim.y = 5;

	pastPress[UP] = false;
	pastPress[DOWN] = false;
	pastPress[RIGHT] = true;
	pastPress[LEFT] = false;
}

void KhoiTaoBot(Snake& snakeBot)
{
	snakeBot.n = 5;
	snakeBot.dot[0].x = 10;
	snakeBot.dot[0].y = 10;
	snakeBot.tt = RIGHT;
}

// map
void MapClear()
{
	TextColor(default_ColorCode);
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoXY(consoleWidth, i);
		putchar(30);
	}// in tường bên phải
	for (int i = 0; i < consoleWidth; i++) {
		gotoXY(i, 0);
		if (i % 2 == 0)  putchar(31);
	}// in tường trên
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoXY(0, i);
		putchar(17);
	}// in tường trái
	for (int i = 0; i <= consoleWidth; i++) {
		gotoXY(i, consoleHeight);
		if (i % 2 == 0)  putchar(16);
	}// in tường dưới

}

void MapEGG()
{
	std::fstream f("map.txt");

	for (int i=0; i<consoleHeight; i++)
		for (int j = 0; j < consoleWidth; j++)
		{
			if (j % 2 == 0)
			{
				int x;
				f >> x;
				if (x == 1)
				{
					TextColor(ColorCode_Yellow);
					gotoXY(j, i);
					map[i][j] = 1;
					putchar(16);
					
				}
				else if (x == 2)
				{
					TextColor(ColorCode_Red);
					gotoXY(j, i);
					putchar('*');
				}
			}
		}
	f.close();

}


void HienThi(Snake snake, HoaQua hq)
{
	//choose map
	if (chooseMap==1 || chooseMap == 3) MapClear();
	else if (chooseMap==2)	MapEGG();

	// in ra hoa quả
	TextColor(ColorCode_Green);
	gotoXY(hq.td.x, hq.td.y);
	putchar('O');

	// in ra con rắn
	//in ra cái đầu.
	TextColor(ColorCode_Pink);
	gotoXY(snake.dot[0].x, snake.dot[0].y);
	putchar(2);
	//in nốt phần thân còn lại.
	for (int i = 1; i <= snake.n; i++)
	{
		gotoXY(snake.dot[i].x, snake.dot[i].y);
		if (i == snake.n)
			putchar(' ');
		else 
			putchar('#');// xóa cái đuôi.
	}
	// in điểm, mạng
	gotoXY(consoleWidth + 5, 0);
	printf("Your Score: %d", snake.n);
	gotoXY(consoleWidth + 5, 1);
	printf("Your Heart: %d", 3);
	gotoXY(consoleWidth + 5, 4);
	printf("Press Space to Return menu!");
	gotoXY(consoleWidth + 5, 5);
	printf("Press Enter to Quit!");
}

void HienThiBot(Snake snakeBot) {
	TextColor(ColorCode_Yellow);
	gotoXY(snakeBot.dot[0].x, snakeBot.dot[0].y);
	putchar(2);
	//in nốt phần thân còn lại.
	for (int i = 1; i <= snakeBot.n; i++)
	{
		gotoXY(snakeBot.dot[i].x, snakeBot.dot[i].y);
		if (i == snakeBot.n)
			putchar(' ');
		else
			putchar('#');// xóa cái đuôi.
	}
}
void DieuKhien_DiChuyen(Snake& snake)
{
	// truyền trạng thái cho đốt cũ
	for (int i = snake.n; i > 0; i--)
		snake.dot[i] = snake.dot[i - 1];

	if (_kbhit()) // nếu phát hiện có phím nhấn vào
	{
		int key = _getch();

		// Chọn lại map
		if (key == ' ') {
			clrscr();
			chooseMap = ChonMap();
			clrscr();
		}
		// Thoát Game
		if (key == 13) exit(0);
		// điều khiển cái đầu mà thôi
		if ((key == 'A' || key == 'a') && pastPress[RIGHT] == false)
		{
			snake.tt = LEFT;
			pastPress[LEFT] = true;
			pastPress[UP] = false;
			pastPress[RIGHT] = false;
			pastPress[DOWN] = false;
		}
		else if ((key == 'D' || key == 'd') && pastPress[LEFT] == false)
		{
			snake.tt = RIGHT;
			pastPress[DOWN] = false;
			pastPress[UP] = false;
			pastPress[LEFT] = false;
			pastPress[RIGHT] = true;
		}
		else if ((key == 'W' || key == 'w') && pastPress[DOWN] == false)
		{
			pastPress[UP] = true;
			pastPress[DOWN] = false;
			pastPress[LEFT] = false;
			pastPress[RIGHT] = false;
			snake.tt = UP;
		}
		else if ((key == 'S' || key == 's') && pastPress[UP] == false)
		{
			snake.tt = DOWN;
			pastPress[RIGHT] = false;
			pastPress[LEFT] = false;
			pastPress[UP] = false;
			pastPress[DOWN] = true;
		}
	}


	if (snake.tt == UP)
	{
		snake.dot[0].y--;
	}
	else if (snake.tt == DOWN )
	{
		snake.dot[0].y++;
	}
	else if (snake.tt == LEFT)
	{
		snake.dot[0].x--;
		
	}
	else if (snake.tt == RIGHT )
	{
		snake.dot[0].x++;
	}
}

void DieuKhien_DiChuyen_Bot(Snake& snakeBot, HoaQua hq) {

	if ((hq.td.x < snakeBot.dot[0].x) && (snakeBot.tt != RIGHT)) snakeBot.tt = LEFT;
	if ((hq.td.x > snakeBot.dot[0].x) && (snakeBot.tt != LEFT)) snakeBot.tt = RIGHT;
	if ((hq.td.y < snakeBot.dot[0].y) && (snakeBot.tt != DOWN)) snakeBot.tt = UP;
	if ((hq.td.y > snakeBot.dot[0].y) && (snakeBot.tt != UP)) snakeBot.tt = DOWN;
	for (int i = snakeBot.n; i > 0; i--)
		snakeBot.dot[i] = snakeBot.dot[i - 1];

	if (snakeBot.tt == UP)
	{
		snakeBot.dot[0].y--;
	}
	else if (snakeBot.tt == DOWN)
	{
		snakeBot.dot[0].y++;
	}
	else if (snakeBot.tt == LEFT)
	{
		snakeBot.dot[0].x--;

	}
	else if (snakeBot.tt == RIGHT)
	{
		snakeBot.dot[0].x++;
	}

}

// trả về 0 nếu không có gì
// trả về -1 nếu thua game
int XuLy(Snake & snake, Snake & snakeBot, HoaQua & hq, int& ThoiGianSleep, int chooseMap)
{
	// xử lý chạm biên không chết.
	// rắn người.
	if (snake.dot[0].x <= 0) 
	{
		snake.dot[0].x = consoleWidth - 1;
		snake.tt = LEFT;
	}
	if (snake.dot[0].x >= consoleWidth) {
		snake.dot[0].x = 1;
		snake.tt = RIGHT;
	}
	if (snake.dot[0].y <= 0) {
		snake.dot[0].y = consoleHeight - 1;
		snake.tt = UP;
	}
	if (snake.dot[0].y >= consoleHeight) {
		snake.dot[0].y = 1;
		snake.tt = DOWN;
	}
	// rắn bot.
	if (snakeBot.dot[0].x <= 0)
	{
		snakeBot.dot[0].x = consoleWidth - 1;
		snakeBot.tt = LEFT;
	}
	if (snakeBot.dot[0].x >= consoleWidth) {
		snakeBot.dot[0].x = 1;
		snakeBot.tt = RIGHT;
	}
	if (snakeBot.dot[0].y <= 0) {
		snakeBot.dot[0].y = consoleHeight - 1;
		snakeBot.tt = UP;
	}
	if (snakeBot.dot[0].y >= consoleHeight) {
		snakeBot.dot[0].y = 1;
		snakeBot.tt = DOWN;
	}
	// Xử lý cắn vào thân	
	for (int i = 1; i < snake.n; i++) {
		if (snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y) {
			if (chooseMap != 3) return -1;// Chết.
			if (chooseMap == 3) {
				for (int j = snake.n; j > i; j--) {
					gotoXY(snake.dot[j].x, snake.dot[j].y);
					putchar(' ');
				}
				snake.n = i;// Không chết.
			}

		}
		// nếu rắn bot cắn rắn người thi thua game.
		if (chooseMap == 4) {
			if (snake.dot[i].x == snakeBot.dot[0].x && snake.dot[i].y == snakeBot.dot[0].y) {
				return -1;
			}
		}

	}
	for (int i = 1; i < snakeBot.n; i++) {
		if (snakeBot.dot[0].x == snakeBot.dot[i].x && snakeBot.dot[0].y == snakeBot.dot[i].y) {// nếu rắn bot tự cắn vào thân thì.
			for (int i = snakeBot.n; i > 0; i--) {
				gotoXY(snakeBot.dot[i].x, snakeBot.dot[i].y);
				putchar(' ');// xóa hết con rắn bot == coi như nó bị chết rồi xuất hiện bot mới.
				snakeBot.n = 1;
				snakeBot.dot[0].x = rand() % (consoleWidth - 1) + 1;
				snakeBot.dot[0].y = rand() % (consoleHeight - 1) + 1;
			}
		}
		if (snake.dot[0].x == snakeBot.dot[i].x && snake.dot[0].y == snakeBot.dot[i].y) {// nếu rắn người cắn vào bot thì bot bị khỏi tạo lại
			for (int j = snakeBot.n; j >= 0; j--) {
				gotoXY(snakeBot.dot[j].x, snakeBot.dot[j].y);
				putchar(' ');// xóa hết con rắn bot == coi như nó bị chết rồi xuất hiện bot mới.
				snakeBot.n = 1;
				snakeBot.dot[0].x = rand() % (consoleWidth - 1) + 1;
				snakeBot.dot[0].y = rand() % (consoleHeight - 1) + 1;
			}
		}
	}
	//xu ly ran can vao map
	if (chooseMap == 2) 
		if (map[snake.dot[0].y][snake.dot[0].x] == 1)
		return -1;
	// rắn người ăn được hoa quả	
	if (snake.dot[0].x == hq.td.x && snake.dot[0].y == hq.td.y)
	{
		for (int i = snake.n + 1; i > 0; i--)
			snake.dot[i] = snake.dot[i - 1];

		snake.n++;

		if (snake.tt == UP)
			snake.dot[0].y--;
		else if (snake.tt == DOWN)
			snake.dot[0].y++;
		else if (snake.tt == LEFT)
			snake.dot[0].x--;
		else if (snake.tt == RIGHT)
			snake.dot[0].x++;


		hq.td.x = rand() % (consoleWidth - 1) + 1;

		hq.td.y = rand() % (consoleHeight - 1) + 1;

			
		if (chooseMap == 2) {
			while (map[hq.td.y][hq.td.x] == 1)
			{
				hq.td.y = rand() % consoleHeight;
			}
		}
		if (ThoiGianSleep > 30)
			ThoiGianSleep -= 10;
	}
	// rắn bot ăn được hoa quả
	if (snakeBot.dot[0].x == hq.td.x && snakeBot.dot[0].y == hq.td.y)
	{
		for (int i = snakeBot.n + 1; i > 0; i--)
			snakeBot.dot[i] = snakeBot.dot[i - 1];

		snakeBot.n++;

		if (snakeBot.tt == UP)
			snakeBot.dot[0].y--;
		else if (snakeBot.tt == DOWN)
			snakeBot.dot[0].y++;
		else if (snakeBot.tt == LEFT)
			snakeBot.dot[0].x--;
		else if (snakeBot.tt == RIGHT)
			snakeBot.dot[0].x++;

		hq.td.x = rand() % (consoleWidth - 1) + 1;

		hq.td.y = rand() % (consoleHeight - 1) + 1;
		//if (ThoiGianSleep > 30)
			//ThoiGianSleep -= 3 * snakeBot.n;
	}
	return 0;
}


int main()
{
	//Init
	Snake snake, snakeBot;
	HoaQua hq;
	int ma = 0;
	int ThoiGianSleep = 150;

	chooseMap = ChonMap();
	clrscr();
	srand(time(NULL));	// khởi tạo bộ sinh số ngẫu nhiên
	KhoiTao(snake, hq);
	KhoiTaoBot(snakeBot);

	if (chooseMap == 5) goto endgame;
	while (1)
	{
		Nocursortype();
		//Bot
		if (chooseMap == 4) {
			HienThiBot(snakeBot);
			DieuKhien_DiChuyen_Bot(snakeBot, hq);
		}
		// hiển thị
		HienThi(snake, hq);

		// điều khiển
		DieuKhien_DiChuyen(snake);

		// xử lý ăn hoa quả, thua game
		ma = XuLy(snake, snakeBot, hq, ThoiGianSleep, chooseMap);

		// thua game, thắng game
		if (ma == -1)	// thua game đó nha
		{
			gotoXY(consoleWidth + 1, 10);
			printf("Ban thua roi, liu liu\n");

			while (_getch() != 13);

			break;
		}

		if (pastPress[LEFT] || pastPress[RIGHT]) Sleep(ThoiGianSleep / 2);
		else Sleep(ThoiGianSleep);
		gotoXY(consoleWidth + 5, 10);
		printf("%d",ThoiGianSleep);

	}
	endgame: return 0;
}