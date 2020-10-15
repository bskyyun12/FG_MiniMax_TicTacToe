#pragma once
#include <iostream>
#include <map>

class Game
{
public:
	static Game* GetInstance();
	static void Release();
	void StartGame();

private:
	static Game* Instance;
	int Board[3][3];
	
	enum BoardSpot
	{
		Empty = 0,
		Player = 5,
		AI = 7,
	};

	BoardSpot CurrentTurn;

	bool bQuit = false;

private:
	Game();
	~Game();

	void CreateBoard();
	void UpdateBoard();

	void CleanScreen();

	void StartTurn(BoardSpot Turn);

	void EndTurn();
	bool HasWon(BoardSpot Turn);
	bool IsDraw();
	void EndGame();
	bool IsGameOver();

	int MiniMax(int Board[3][3], bool AITurn, int Depth);
};

