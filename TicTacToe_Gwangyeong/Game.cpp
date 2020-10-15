#include "Game.h"

Game* Game::Instance = nullptr;

Game::Game()
{

}

Game::~Game()
{

}

Game* Game::GetInstance()
{
	if (Instance == nullptr) { Instance = new Game(); }

	return Instance;
}

void Game::Release()
{
	delete Instance;
	Instance = nullptr;
}

void Game::CreateBoard()
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			Board[row][col] = Empty;
		}
	}

	UpdateBoard();
}

void Game::UpdateBoard()
{
	CleanScreen();
	std::cout << "     0   1   2  " << std::endl;

	for (int row = 0; row < 3; row++)
	{
		std::cout << "   -------------" << std::endl;
		std::cout << " " << row << " -";
		for (int col = 0; col < 3; col++)
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
	std::cout << "   -------------" << std::endl;
}

void Game::CleanScreen()
{
	system("cls");
}

void Game::StartTurn(BoardSpot Turn)
{
	CurrentTurn = Turn;

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
			else if (Row > 2 || Row < 0)
			{
				std::cout << " Enter between 0 and 2." << std::endl;
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
			else if (Col > 2 || Col < 0)
			{
				std::cout << " Enter between 0 and 2." << std::endl;
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
		EndTurn();
	}
	else // AI
	{
		std::cout << " AI's Turn" << std::endl;

		int BestScore = -1000;
		int BestRow;
		int BestCol;

		for (int Row = 0; Row < 3; Row++)
		{
			for (int Col = 0; Col < 3; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = AI;
					int Score = MiniMax(Board, false, 0);
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

		EndTurn();
	}
}

int Game::MiniMax(int Board[3][3], bool IsMax, int Depth)
{
	if (HasWon(Player)) { return -1; }
	if (HasWon(AI)) { return 1; }
	if (IsDraw()) { return 0; }

	if (IsMax)
	{
		int BestScore = -1000;
		for (int Row = 0; Row < 3; Row++)
		{
			for (int Col = 0; Col < 3; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = AI;
					int Score = MiniMax(Board, false, Depth + 1);
					BestScore = std::max(BestScore, Score);
					Board[Row][Col] = Empty;
				}
			}
		}
		return BestScore;
	}
	else
	{
		int BestScore = 1000;
		for (int Row = 0; Row < 3; Row++)
		{
			for (int Col = 0; Col < 3; Col++)
			{
				if (Board[Row][Col] == Empty) // empty spot
				{
					Board[Row][Col] = Player;
					int Score = MiniMax(Board, true, Depth + 1);
					BestScore = std::min(BestScore, Score);
					Board[Row][Col] = Empty;
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
	if (CurrentTurn == Player)
	{
		StartTurn(AI);
	}
	else
	{
		//std::cout << " Press any key to continue...";
		//std::cin.ignore();
		//std::cin.get();

		StartTurn(Player);
	}
}

bool Game::HasWon(BoardSpot Turn)
{
	if (
		// Horizontal
		Board[0][0] + Board[0][1] + Board[0][2] == Turn * 3 ||
		Board[2][0] + Board[2][1] + Board[2][2] == Turn * 3 ||
		Board[1][0] + Board[1][1] + Board[1][2] == Turn * 3 ||
		// Vertical
		Board[0][0] + Board[1][0] + Board[2][0] == Turn * 3 ||
		Board[0][1] + Board[1][1] + Board[2][1] == Turn * 3 ||
		Board[0][2] + Board[1][2] + Board[2][2] == Turn * 3 ||
		// Diagonal
		Board[0][0] + Board[1][1] + Board[2][2] == Turn * 3 ||
		Board[2][0] + Board[1][1] + Board[0][2] == Turn * 3
		)
	{
		return true;
	}

	return false;
}

bool Game::IsDraw()
{
	for (int Row = 0; Row < 3; Row++)
	{
		for (int Col = 0; Col < 3; Col++)
		{
			if (Board[Row][Col] == Empty)
			{
				return false;
			}
		}
	}

	return true;
}

bool Game::IsGameOver()
{
	if (HasWon(Player)) { return true; }
	if (HasWon(AI)) { return true; }
	if (IsDraw()) { return true; }

	return false;
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
	// draw map
	CreateBoard();

	char Answer;
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