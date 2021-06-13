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

#define Rows 50
#define Cols 150
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKET 77
#define UPKEY2 119
#define DOWNKEY2 115
#define LEFTKEY2 97
#define RIGHTKEY2 100
#define UPKEY3 105
#define DOWNKEY3 107
#define LEFTKEY3 106
#define RIGHTKEY3 108
int SnakeCount = 2;
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
bool IsValidFood(Position &FPosition, Position& FOPposition, const Snake& S, const Snake& S2)
{
	if (FPosition == FOPposition)
		return false;
	for (int i = 0; i < S.Ps.size(); i++)
	{
		if (S.Ps[i] == FPosition)
			return false;
	}
	for (int i = 0; i < S2.Ps.size(); i++)
	{
		if (S2.Ps[i] == FPosition)
			return false;
	}
	return true;
}


void GenerateFood(Position &FPosition, Position& FOPposition, const Snake& S, const Snake& S2)
{
	do {
		FPosition.ri = rand() % Rows;
		FPosition.ci = rand() % Cols;
		FOPposition.ri = rand() % Rows;
		FOPposition.ci = rand() % Cols;
	} while (!IsValidFood(FPosition, FOPposition, S, S2));
}
void Init(Snake& S, Position& Fposition, Position& FOPposition)
{
	S.Ps.resize(3);
	S.Ps[0].ri = Rows / 2;
	S.Ps[0].ci = Cols / 2;
	S.Ps[1].ri = Rows / 2;
	S.Ps[1].ci = Cols / 2 + 1;
	S.Ps[2].ri = Rows / 2;
	S.Ps[2].ci = Cols / 2 + 2;
	S.Sym = '+';
	S.Score = 0;
	S.DownKey = DOWNKEY;
	S.UpKey = UPKEY;
	S.LeftKey = LEFTKEY;
	S.RightKey = RIGHTKET;
	S.Direction = LEFT;
	//GenerateFood(Fposition, FOPposition, S);
}
void Init2(Snake& S2, Position& Fposition, Position& FOPposition)
{
	S2.Ps.resize(3);
	S2.Ps[0].ri = Rows / 3;
	S2.Ps[0].ci = Cols / 3;
	S2.Ps[1].ri = Rows / 3;
	S2.Ps[1].ci = Cols / 3 + 1;
	S2.Ps[2].ri = Rows / 3;
	S2.Ps[2].ci = Cols / 3 + 2;
	S2.Sym = 'o';
	S2.Score = 0;
	S2.DownKey = DOWNKEY2;
	S2.UpKey = UPKEY2;
	S2.LeftKey = LEFTKEY2;
	S2.RightKey = RIGHTKEY2;
	S2.Direction = RIGHT;
	//GenerateFood(Fposition, FOPposition, S2);
}
void SnakeKilling(Snake& S)
{
	if (S.Ps[0].ri == 0 || S.Ps[0].ci == 0 || S.Ps[0].ri == 50 || S.Ps[0].ci == 150)
	{
		S.Sym = ' ';
		SnakeCount--;
	}
	if (SnakeCount == 1)
	{
		if (S.Sym == 'o')
			cout << "player 1 wins" << endl;
		else if (S.Sym == '+')
			cout << "player 2 wins" << endl;

	}
	
}

void UpdateDirection(Snake& S, int& Key)
{
	if (Key == S.LeftKey && S.Direction != RIGHT)
	{
		S.Direction = LEFT;
	}
	else if (Key == S.RightKey && S.Direction != LEFT)
	{
		S.Direction = RIGHT;
	}
	else if (Key == S.UpKey && S.Direction != DOWN)
	{
		S.Direction = UP;
	}
	else if (Key == S.DownKey && S.Direction != UP)
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
	for (int i = S.Ps.size() - 1; i > 0; i--)
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
	SnakeKilling(S);
	
}
void DisplayFood(Position FPosition, Position FOPposition, char fsym, char FOPsym)
{
	gotoRowCol(FPosition.ri, FPosition.ci);
	cout << fsym;
	gotoRowCol(FOPposition.ri, FOPposition.ci);
	cout << FOPsym;
}
void DislpayScore(Snake S)
{
	gotoRowCol((Rows / 2) - 2, (Cols + 2) - 1);
	cout << "Score of player 2";
	gotoRowCol((Rows/2) - 1, (Cols+2) - 1);
	cout << S.Score;
}
void DislpayScore2(Snake S2)
{
	gotoRowCol((Rows / 2) - 5, (Cols + 2) - 1);
	cout << "Scoreof player 1";
	gotoRowCol((Rows / 2) - 4, (Cols + 2) - 1);
	cout << S2.Score;
}
int FoodEaten(Position FPostion,Position FOPposition, Snake S)
{
	if (FPostion == S.Ps[0])
		return 1;
	if (FOPposition == S.Ps[0])
		return 2;

	return 3;
}
void ExtendSnake(Snake& S, Position TPosition) {
	S.Ps.push_back(TPosition);
}
void ReducePoints(Snake& S, Snake& S2, Position Tposition, Position Tposition2)
{
	if (S.Ps[0] == S2.Ps[S2.Ps.size()-1])
	{
		S.Score--;
		//S2.Score = 15;
	}
	if (S2.Ps[0] == S.Ps[S.Ps.size() - 1])
	{
		S2.Score--;
		//S.Score = 15;
	}
	
}

int main() {
	int sec = 0, min = 0, hr = 0;
	gotoRowCol(1, 148);
	cout<<"\r" << hr << ":" << min << ":" << sec;
	for (int i = 0; i < Rows; ++i) {   //boundary
		for (int j = 0; j < Cols; ++j) {
			if (i == 0 || i == Rows - 1) {
				gotoRowCol(i, j);
				cout << "=";
			}
			else if (j == 0 || j == Cols - 1) {
				gotoRowCol(i, j);
				cout << "|";
			}

		}
	}

	Snake S,S2,S3;
	Position FPosition;
	Position FOPposition; //food of point position
	Position TPosition, TPosition2, TPosition3;
	char Fsym = '*';
	char FOPsym = '&';
	Init(S, FPosition, FOPposition);
	Init2(S2, FPosition, FOPposition);
	GenerateFood(FPosition, FOPposition, S, S2);
	DisplayFood(FPosition, FOPposition, Fsym, FOPsym);
	DisplaySnake(S, S.Sym);
	DisplaySnake(S2, S2.Sym);

	TPosition = S.Ps[S.Ps.size() - 1];
	TPosition2 = S2.Ps[S2.Ps.size() - 1];

	while (true)
	{
		sec += 1;
	    if (sec == 60)
		{
			sec = 0;
			min += 1;
			if (min == 60)
			{
				min = 0;
				hr++;
			}
		}
		gotoRowCol(1, 148);
		cout << "\r"<<setw(2) << hr << ":" <<setw(2)<< min << ":" <<setw(2)<< sec;
		DislpayScore(S);
		DislpayScore2(S2);

		if (_kbhit())
		{
			int Key = _getch();
			if (Key == 224)
			{
				int Key = _getch();
				UpdateDirection(S, Key);
			}
			//WASD
		    if (Key == 115 || Key == 119 || Key == 97 || Key == 100) {
				//Key = _getch();
				UpdateDirection(S2, Key);
			}
			// IJKL
			if (Key == 105 || Key == 106 || Key == 107 || Key == 108) {
				Key = _getch();
				UpdateDirection(S3, Key);
			}
		}
		Sleep(100);
		DisplaySnake(S, ' ');
		DisplaySnake(S2, ' ');
		TPosition = S.Ps[S.Ps.size() - 1];
		TPosition2 = S2.Ps[S2.Ps.size() - 1];
		SnakeMove(S);
		ReducePoints(S, S2, TPosition,TPosition2);
		//ReducePoints(S, S2, TPosition, TPosition2);
		//DislpayScore(S);
		SnakeMove(S2);
		if (SnakeCount == 1)
		{
			system("pause");
			return 0;

		}

		ReducePoints(S, S2, TPosition, TPosition2);

		
		

		/*ReducePoints(S, S2, TPosition, TPosition2);
		ReducePoints(S, S2, TPosition, TPosition2);
		ReducePoints(S, S2, TPosition, TPosition2);*/
		//DislpayScore2(S2);

		DisplaySnake(S, S.Sym);
		DisplaySnake(S2, S2.Sym);
		if (FoodEaten(FPosition,FOPposition, S)==1)
		{
			ExtendSnake(S, TPosition);
			GenerateFood(FPosition,FOPposition, S,S2);
			DisplayFood(FPosition, FOPposition, Fsym, FOPsym);
		}
		if (FoodEaten(FPosition, FOPposition, S) == 2)
		{
			//ExtendSnake(S, TPosition);
			if (FOPsym == -37)
				S.Score = S.Score + 2;            ////EXTRA FUNCTIONALITY
			else
				S.Score++;
			if (S.Score == 3)
			{
				FOPsym = -37;
			}
			DislpayScore(S);
			GenerateFood(FPosition, FOPposition, S, S2);
			DisplayFood(FPosition, FOPposition, Fsym, FOPsym);
		}
		DisplaySnake(S, S.Sym);
		if (FoodEaten(FPosition, FOPposition, S2) == 1)
		{
			ExtendSnake(S2, TPosition2);
			GenerateFood(FPosition, FOPposition, S, S2);
			DisplayFood(FPosition, FOPposition, Fsym, FOPsym);
		}
		if (FoodEaten(FPosition, FOPposition, S2) == 2)
		{
			//ExtendSnake(S, TPosition);
			if (FOPsym == -37)                    ///////EXTRA FUNCTIONALITY
				S.Score = S.Score + 2;
			else
				S.Score++;
			if (S.Score == 3)
			{
				FOPsym = -37;
			}
			DislpayScore2(S2);
			GenerateFood(FPosition, FOPposition, S, S2);
			DisplayFood(FPosition, FOPposition, Fsym, FOPsym);
		}
		DisplaySnake(S2, S2.Sym);

	}





	system("pause");
	return 0;
}