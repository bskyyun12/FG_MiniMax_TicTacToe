#pragma once
#include <iostream>
#include <map>

class Game
{
public:
	Game();
	~Game();
	void StartGame();
	void ReleaseGame();

private:
	//static const int BoardSize = 5;
	//int Board[BoardSize][BoardSize];

	int** Board;
	int BoardSize = 5;
	int EmptySlotCount = BoardSize * BoardSize;
	int Difficulty = 7;

	enum Turn
	{
		Empty = 0,
		Player = 555,
		AI = 777,
	};

	Turn CurrentTurn;

	bool bQuit = false;

private:

	void CreateBoard();
	void UpdateBoard();

	void CleanScreen();

	void StartTurn(Turn TurnToStart);

	void EndTurn();
	bool HasWon(Turn TurnToCheck);
	bool IsDraw();
	void EndGame();
	bool IsGameOver();

	int MiniMax(int** Board, bool AITurn, int Depth, int Alpha, int Beta);
};

