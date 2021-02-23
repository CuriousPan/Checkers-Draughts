#include "Board.h"
#include "ManFactory.h"


Board::Board() 
{

	std::map<Location, Man*> menMap = ManFactory::getMen();
	for (int row = 1; row <= BOARD_SIZE; row++)
	{
		squares.push_back(Row());
		for (int column = 1; column <= BOARD_SIZE; column++)
		{
			Location currentLocation(File(column), row);
			SquareColor currentSquareColor = SquareColor::WHITE;
			if ((column + row) % 2 == 0)		//lol the bug was here, forgot about brackets
			{
				currentSquareColor = SquareColor::BLACK;
			}
			Square* currentSquare = new Square(currentLocation, currentSquareColor);
			for (std::map<Location, Man*>::iterator it = menMap.begin(); it != menMap.end(); it++)
			{
				if (it->first == currentLocation)
				{
					Man* currentMan = (it)->second;
					currentMan->setCurrentSquare(currentSquare);
					currentSquare->setCurrentMan(currentMan);
					currentSquare->setIsOccupied();
					if (currentMan->getColor() == ManColor::WHITE)
					{
						whitePieces.push_back(currentMan);
					}
					else
					{
						blackPieces.push_back(currentMan);
					}
					break;
				}
			}
			locationSquareMap.insert(std::make_pair(currentLocation, currentSquare));
			squares.at(row-1).push_back(currentSquare);
		}
	}	
}

//Board::Board(Board& board)
//{
//	//cctor
//}

Board::~Board()
{
	//dtor
}

void Board::printBoard()
{
	std::cout << "\n    ";
	for (int i = 1; i <= BOARD_SIZE; i++)
	{
		switch (File(i))
		{
		case File::A : std::cout << "A "; break;
		case File::B : std::cout << "B "; break;
		case File::C : std::cout << "C "; break;
		case File::D : std::cout << "D "; break;
		case File::E : std::cout << "E "; break;
		case File::F : std::cout << "F "; break;
		case File::G : std::cout << "G "; break;
		case File::H : std::cout << "H "; break;
		default: std::cout << " "; break;
		}
	}
	
	std::cout << std::endl << std::endl;
	for (int i = squares.size() - 1; i != -1; i--)
	{
		std::cout << i + 1 << "   ";
		for(int g = 0; g < squares.at(i).size(); g++)
		{
			if (squares.at(i).at(g)->getIsOccupied())
			{
				Man* currentMan = squares.at(i).at(g)->getCurrentMan();
				if (currentMan->getColor() == ManColor::WHITE)
				{
					if (currentMan->getIsKing())
					{
						std::cout << "W ";
					}
					else
					{
						std::cout << "w ";
					}
				}
				else
				{
					if (currentMan->getIsKing())
					{
						std::cout << "B ";
					}
					else
					{
						std::cout << "b ";
					}
				}
			}
			else
			{
				std::cout << "_ ";
			}
		}
		std::cout << std::endl;
	}
}

std::map<Location, Square*> Board::getLocationSquareMap()
{
	return locationSquareMap;
}

std::vector<Man*>* Board::getWhitePieces()
{
	return &whitePieces;
}

std::vector<Man*>* Board::getBlackPieces()
{
	return &blackPieces;
}

