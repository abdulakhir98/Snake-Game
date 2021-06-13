#include<iostream> 
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<iomanip>
#include<vector>
using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

//up: -32 72
//down: -32 80
//left -32 75
//right -32 77
#define Rows 90
#define Cols 90
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKET 77
struct Position {
	int ri, ci;
	bool operator==(const Position P)const
	{
		return ri == P.ri && ci == P.ci;
	}
};
struct Snake {
	int Score;
	vector<Position> Ps;
	char Sym;
	int Direction;
	int LeftKey;
	int RightKey;
	int UpKey;
	int DownKey;
};
bool IsValidFood(Position &FPosition, const Snake& S)
{
	for (int i = 0; i < S.Ps.size(); i++)
	{
		if (S.Ps[i] == FPosition)
			return false;
	}
	return true;
}


void GenerateFood(Position &FPosition, const Snake& S)
{
	do {
		FPosition.ri = rand() % Rows;
		FPosition.ci = rand() % Cols;
	} while (!IsValidFood(FPosition, S));
}
void Init(Snake& S, Position& Fposition)
{
	S.Ps.resize(3);
	S.Ps[0].ri = Rows / 2;
	S.Ps[0].ci = Cols / 2;
	S.Ps[1].ri = Rows / 2;
	S.Ps[1].ci = Cols / 2+1;
	S.Ps[2].ri = Rows / 2;
	S.Ps[2].ci = Cols / 2+2;
	S.Sym = -37;
	S.Score = 0;
	S.DownKey = DOWNKEY;
	S.UpKey = UPKEY;
	S.LeftKey = LEFTKEY;
	S.RightKey = RIGHTKET;
	S.Direction = LEFT;
	GenerateFood(Fposition,S);
}

void UpdateDirection(Snake& S, int& Key)
{
	if (Key == S.LeftKey && S.Direction!= RIGHT)
	{
		S.Direction = LEFT;
	}
	else if (Key == S.RightKey && S.Direction!=LEFT)
	{
		S.Direction = RIGHT;
	}
	else if (Key == S.UpKey && S.Direction!= DOWN)
	{
		S.Direction = UP;
	}
	else if (Key == S.DownKey && S.Direction!= UP)
	{
		S.Direction = DOWN;
	}
}
void DisplaySnake(const Snake& S, char s)
{
	for (int i = 0; i < S.Ps.size(); i++)
	{
		gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
		cout << s;
	}
}
void SnakeMove(Snake& S)
{
	for (int i = S.Ps.size()-1; i > 0; i--)
	{
		S.Ps[i] = S.Ps[i - 1];
	}
	switch (S.Direction)
	{
	case UP:
		S.Ps[0].ri--;
		if (S.Ps[0].ri == -1)
			S.Ps[0].ri = Rows - 1;
		break;
	case DOWN:
		S.Ps[0].ri++;
		if (S.Ps[0].ri == Rows)
			S.Ps[0].ri = 0;
		break;
	case LEFT:
		S.Ps[0].ci--;
		if (S.Ps[0].ci == -1)
			S.Ps[0].ci = Cols - 1;
		break;
	case RIGHT:
		S.Ps[0].ci++;
		if (S.Ps[0].ci == Cols)
			S.Ps[0].ci = 0;
		break;
	}
}
void DisplayFood(Position FPosition, char fsym)
{
	gotoRowCol(FPosition.ri, FPosition.ci);
	cout << fsym;
}
bool FoodEaten(Position FPostion, Snake S)
{
	return FPostion == S.Ps[0];
}
void ExtendSnake(Snake& S, Position TPosition) {
	S.Ps.push_back(TPosition);
}
int main() {

	Snake S;
	Position FPosition;
	Position TPosition;
	char Fsym='&';
	Init(S,FPosition);
	DisplayFood(FPosition, Fsym);
	DisplaySnake(S,S.Sym);
	
	TPosition = S.Ps[S.Ps.size() - 1];

	while (true)
	{

		if (_kbhit())
		{
			int Key = _getch();
			if (Key == 224)
			{
				int Key = _getch();
				UpdateDirection(S, Key);
			}
		}
		Sleep(100);
		DisplaySnake(S, ' ');
		TPosition = S.Ps[S.Ps.size() - 1];
		SnakeMove(S);
		DisplaySnake(S, S.Sym);
		if (FoodEaten(FPosition, S))
		{
			ExtendSnake(S, TPosition);
			GenerateFood(FPosition, S);
			DisplayFood(FPosition, Fsym);
		}
		DisplaySnake(S, S.Sym);
	}

	
	
	

	system("pause");
	return 0;
}