
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include "console.h"
#include <iostream>
#include <fstream>

#define consoleWidth	80
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
};


struct Snake
{
	ToaDo dot[31];
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
	printf("2. Map\n");
	printf("3. Never Die\n");
	printf("4. Exit");
	int i = 0;
	scanf("%d", &i);
	return i;
}

void KhoiTao(Snake& snake, HoaQua& hq)
{
	snake.n = 1;
	snake.dot[0].x = 1;
	snake.dot[0].y = 1;

	snake.tt = RIGHT;

	hq.td.x = 2;
	hq.td.y = 1;

	pastPress[UP] = false;
	pastPress[DOWN] = false;
	pastPress[RIGHT] = true;
	pastPress[LEFT] = false;
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
	if (chooseMap==1) MapClear();
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
}


void DieuKhien_DiChuyen(Snake& snake)
{
	// truyền trạng thái cho đốt cũ
	for (int i = snake.n; i > 0; i--)
		snake.dot[i] = snake.dot[i - 1];

	if (_kbhit()) // nếu phát hiện có phím nhấn vào
	{
		int key = _getch();

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


// trả về 0 nếu không có gì
// trả về -1 nếu thua game
int XuLy(Snake & snake, HoaQua & hq, int& ThoiGianSleep)
{
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
		

	for (int i = 1; i < snake.n; i++)
		if ((snake.dot[0].x == snake.dot[i].x &&
			snake.dot[0].y == snake.dot[i].y) || 
			(map[snake.dot[0].y][snake.dot[0].x] == 1))
			return -1;

	if (snake.dot[0].x == hq.td.x && snake.dot[0].y == hq.td.y)
	{
		// ăn được hoa quả
		// việc ăn được hoa quả rồi mới chèn thêm cái đầu mới thì thật sự chưa giải quyết tốt cho lắm
		// các bạn cần suy nghĩ thêm để tốt hơn
		
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


		hq.td.x = rand() % consoleWidth;
		while ((hq.td.x >= (consoleWidth - 1)) || (hq.td.x == 0))
		{
			hq.td.x = rand() % consoleWidth;
			//std::cout << hq.td.x;
		}
		hq.td.y = rand() % consoleHeight;
		while ((hq.td.y >= (consoleWidth - 1)) || (hq.td.y == 0) || (map[hq.td.y][hq.td.x]==1))
		{
			hq.td.y = rand() % consoleWidth;
			
		}
	
		//if (ThoiGianSleep > 30)      em khong hieu doan if nay co tac dung gi?
		//	ThoiGianSleep -= 20;
	}
	return 0;
}


int main()
{
	//Init
	Snake snake;
	HoaQua hq;
	int ma = 0;
	int ThoiGianSleep = 100;

	chooseMap = ChonMap();
	clrscr();
	srand(time(NULL));	// khởi tạo bộ sinh số ngẫu nhiên
	KhoiTao(snake, hq);
	while (1)
	{
		Nocursortype();
		// hiển thị
		HienThi(snake, hq);

		// điều khiển
		DieuKhien_DiChuyen(snake);

		// xử lý ăn hoa quả, thua game
		ma = XuLy(snake, hq, ThoiGianSleep);

		// thua game, thắng game
		if (ma == -1)	// thua game đó nha
		{
			gotoXY(consoleWidth + 1, 10);
			printf("Ban thua roi, liu liu");

			while (_getch() != 13);

			break;
		}

		if (pastPress[LEFT] || pastPress[RIGHT]) Sleep(ThoiGianSleep / 2);
		else Sleep(ThoiGianSleep);
	}
	return 0;
}