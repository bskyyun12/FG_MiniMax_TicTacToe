#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::ReleaseGame()
{
	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < BoardSize; ++i)
	{
		delete[] Board[i];
	}
	delete[] Board;
}

void Game::CreateBoard()
{
	for (int row = 0; row < BoardSize; row++)
	{
		for (int col = 0; col < BoardSize; col++)
		{
			Board[row][col] = Empty;
		}
	}

	UpdateBoard();
}

void Game::UpdateBoard()
{
	CleanScreen();
	std::cout << "     ";
	for (int i = 0; i < BoardSize; i++)
	{
		std::cout << i << "   ";
	}
	std::cout << std::endl;

	for (int row = 0; row < BoardSize; row++)
	{
		std::cout << "   -";
		for (int i = 0; i < BoardSize; i++)
		{
			std::cout << "----";
		}
		std::cout << std::endl;

		std::cout << " " << row << " -";
		for (int col = 0; col < BoardSize; col++)
		{
			if (Board[row][col] == Empty) // Empty
			{
				std::cout << " " << " " << " -";
			}
			else if (Board[row][col] == Player) // Player
			{
				std::cout << " " << "O" << " -";
			}
			else if (Board[row][col] == AI) // AI
			{
				std::cout << " " << "X" << " -";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "   -";
	for (int i = 0; i < BoardSize; i++)
	{
		std::cout << "----";
	}
	std::cout << std::endl;
}

void Game::CleanScreen()
{
	//system("cls");
}

void Game::StartTurn(Turn TurnToStart)
{
	CurrentTurn = TurnToStart;

	std::cout << "\n --------------------------" << std::endl;
	if (CurrentTurn == Player) // Player
	{
		std::cout << " Player's Turn" << std::endl;

		int Row;
		int Col;

		while (1)
		{
			std::cout << " Row: ";
			std::cin >> Row;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << " Please Enter a Number." << std::endl;
			}
			else if (Row > BoardSize - 1 || Row < 0)
			{
				std::cout << " Enter between 0 and " << BoardSize - 1 << "." << std::endl;
			}
			else
			{
				break;
			}
		}

		while (1)
		{
			std::cout << " Col: ";
			std::cin >> Col;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << " Please Enter a Number." << std::endl;
			}
			else if (Col > BoardSize - 1 || Col < 0)
			{
				std::cout << " Enter between 0 and " << BoardSize - 1 << "." << std::endl;
			}
			else
			{
				break;
			}
		}

		if (Board[Row][Col] != Empty)
		{
			CleanScreen();
			UpdateBoard();
			std::cout << " (" << Row << ", " << Col << ") is not an empty place." << std::endl;
			StartTurn(Player);
			return;
		}

		Board[Row][Col] = Player;
		EmptySlotCount--;
		EndTurn();
	}
	else // AI
	{
		std::cout << " AI's Turn" << std::endl;

		int BestScore = -1000;
		int BestRow;
		int BestCol;

		for (int Row = 0; Row < BoardSize; Row++)
		{
			for (int Col = 0; Col < BoardSize; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = AI;
					int Score = MiniMax(Board, false, 0, -9999, 9999);
					Board[Row][Col] = Empty;
					std::cout << " Score: " << Score << ", (" << Row << ", " << Col << ")" << std::endl;

					if (Score > BestScore)
					{
						BestScore = Score;
						BestRow = Row;
						BestCol = Col;
						std::cout << " BestScore: " << BestScore << ", (" << Row << ", " << Col << ")" << std::endl;
					}
				}
			}
		}

		Board[BestRow][BestCol] = AI;
		EmptySlotCount--;
		EndTurn();
	}
}

int Game::MiniMax(int** Board, bool IsMax, int Depth, int Alpha, int Beta)
{
	if (Depth >= Difficulty)
	{
		return 0;
	}
	if (HasWon(Player)) { return -10 + Depth; }
	if (HasWon(AI)) { return 10 - Depth; }
	if (IsDraw()) { return 0; }

	if (IsMax)
	{
		int BestScore = -1000;
		for (int Row = 0; Row < BoardSize; Row++)
		{
			for (int Col = 0; Col < BoardSize; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = AI;
					int Score = MiniMax(Board, false, Depth + 1, Alpha, Beta);
					Board[Row][Col] = Empty;
					BestScore = std::max(BestScore, Score);
					Alpha = std::max(Alpha, BestScore);
					if (Alpha >= Beta)
					{
						break;
					}
				}
			}
		}
		return BestScore;
	}
	else
	{
		int BestScore = 1000;
		for (int Row = 0; Row < BoardSize; Row++)
		{
			for (int Col = 0; Col < BoardSize; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = Player;
					int Score = MiniMax(Board, true, Depth + 1, Alpha, Beta);
					Board[Row][Col] = Empty;
					BestScore = std::min(BestScore, Score);
					Beta = std::min(Beta, BestScore);
					if (Alpha >= Beta)
					{
						break;
					}
				}
			}
		}
		return BestScore;
	}
}

void Game::EndTurn()
{
	UpdateBoard();

	// Check GameOver
	if (IsGameOver())
	{
		EndGame();
		return;
	}

	// Next turn
	StartTurn(CurrentTurn == Player ? AI : Player);
}

bool Game::HasWon(Turn TurnToCheck)
{
	int Sum = 0;
	// Horizontal
	for (int Row = 0; Row < BoardSize; Row++)
	{
		Sum = 0;
		for (int Col = 0; Col < BoardSize; Col++)
		{
			Sum += Board[Row][Col];
		}

		if (Sum == TurnToCheck * BoardSize)
		{
			return true;
		}
	}

	// Vertical
	for (int Row = 0; Row < BoardSize; Row++)
	{
		Sum = 0;
		for (int Col = 0; Col < BoardSize; Col++)
		{
			Sum += Board[Col][Row];
		}

		if (Sum == TurnToCheck * BoardSize)
		{
			return true;
		}
	}

	// Diagonal 1
	Sum = 0;
	for (int i = 0; i < BoardSize; i++)
	{		
		Sum += Board[i][i];
	}
	if (Sum == TurnToCheck * BoardSize)
	{
		return true;
	}

	// Diagonal 2
	Sum = 0;
	for (int i = 0; i < BoardSize; i++)
	{
		Sum += Board[i][BoardSize - 1 - i];
	}
	if (Sum == TurnToCheck * BoardSize)
	{
		return true;
	}

	return false;
}

bool Game::IsDraw()
{
	return EmptySlotCount == 0;
}

bool Game::IsGameOver()
{
	return HasWon(Player) || HasWon(AI) || IsDraw();
}

void Game::EndGame()
{
	UpdateBoard();
	std::cout << " ----------------------------" << std::endl;

	if (IsDraw())
	{
		std::cout << " Draw!" << std::endl;
	}
	else if (CurrentTurn == Player)
	{
		std::cout << " Player won!" << std::endl;
	}
	else
	{
		std::cout << " AI won!" << std::endl;
	}
	std::cout << " ----------------------------" << std::endl;


	char Answer;
	while (1)
	{
		std::cout << " Would you like to play again? (Y/N)" << std::endl;
		std::cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			StartGame();
			break;
		}
		else if (Answer == 'N' || Answer == 'n')
		{
			std::cout << " GG." << std::endl;
			break;
		}
		else
		{
			std::cout << " Please Enter Y or N." << std::endl;
		}
	}
}

void Game::StartGame()
{
	char Answer;
	while (1)
	{
		std::cout << "\n Please Set the board's size. (3 ~ 5)" << std::endl;
		std::cin >> Answer;
		int AnswerInt = (int)Answer - 48;
		if (AnswerInt > 5 || AnswerInt < 3)
		{
			std::cout << " Please Enter Between 3 and 5." << std::endl;
		}
		else
		{
			// Allocate memory
			BoardSize = AnswerInt;
			Board = new int*[BoardSize];
			for (int i = 0; i < BoardSize; ++i)
			{
				Board[i] = new int[BoardSize];
			}
			CreateBoard();
			break;
		}
	}


	while (1)
	{
		std::cout << "\n Please Set the difficulty. (1 ~ 8)" << std::endl;
		std::cin >> Answer;
		int AnswerInt = (int)Answer - 48;
		if (AnswerInt > 8 || AnswerInt < 1)
		{
			std::cout << " Please Enter Between 1 and 8." << std::endl;
		}
		else
		{
			Difficulty = AnswerInt;
			break;
		}
	}

	while (1)
	{
		std::cout << "\n Would you like go first? (Y/N)" << std::endl;
		std::cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			StartTurn(Player);
			break;
		}
		else if (Answer == 'N' || Answer == 'n')
		{
			StartTurn(AI);
			break;
		}
		else
		{
			std::cout << " Please Enter Y or N." << std::endl;
		}
	}
}