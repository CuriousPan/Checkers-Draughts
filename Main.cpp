#include "Board.h"
#include "Man.h"
#include "LocationFactory.h"
#include <algorithm> //count_if

bool hasToCapture(Man* man);

int main()
{
	std::cout << "+-----------------------------------------------+\n" <<
		"|						|\n" <<
		"|	 Hello, it's my first game		|\n" <<
		"|	 of checkers. Please, try it		|\n" <<
		"|	 and enjoy (I hope so).			|\n" <<
		"|	 					|\n" <<
		"|	 P.S. I would really appriciate		|\n" <<
		"|	 if you spot some bug and notify	|\n" <<
		"|	 me about it.				|\n" <<
		"|						|\n" <<
		"+-----------------------------------------------+\n" << std::endl;
		
	std::cout << "The game is played according to Russian checkers rules." << std::endl;
	std::cout << "To make a move enter squares in the following form: E3 D4." << std::endl;
	std::cout << "Multi-take moves have to be made step by step." << std::endl;
	std::cout << "To exit the game press CTRL + C." << std::endl;

 	Board board;
	bool running = true;
	board.printBoard();
	std::string from;
	std::string to;
	ManColor currentTurn = ManColor::WHITE;
	while (running)
	{
		if (currentTurn == ManColor::WHITE) std::cout << "\nWhite has current move" << std::endl;
		else std::cout << "\nBlack has current move" << std::endl;

		std::cout << "Enter your move: ";
		std::cin >> from;
		std::cin >> to;
		if (from.size() != 2 && to.size() != 2)
		{
			continue;
		}
		int fromFile = std::toupper(from[0]) - 64;
		int fromRank = from[1] - 48;
		int toFile = std::toupper(to[0]) - 64;
		int toRank = to[1] - 48;

		try {
			if (fromFile > 8 || fromFile < 1 ||
				fromRank > 8 || fromRank < 1 ||
				toFile > 8 || toFile < 1 ||
				toRank > 8 || toRank < 1)
			{
				throw std::exception("File or rank out of range");
			}
		}
		catch (std::exception)
		{
			std::cout << "Not a valid move: No such square(s) on the board" << std::endl;
			continue;
		}

		
		
		std::map<Location, Square*> _locationSquareMap = board.getLocationSquareMap();
	
		Square* fromSquare;
		Square* toSquare;
		fromSquare = _locationSquareMap.at(Location((File)fromFile, fromRank));
		if (!fromSquare->getIsOccupied())
		{
			std::cout << "Not a valid move: No piece at the square you want to move from" << std::endl;
			continue;
		}
		
		if (fromSquare->getCurrentMan()->getColor() == ManColor::WHITE)
		{
			for (std::vector<Man*>::iterator itW = board.getWhitePieces()->begin(); itW != board.getWhitePieces()->end(); itW++)
			{
				(*itW)->getValidMoves(&board);
			}
		}
		else
		{
			for (std::vector<Man*>::iterator itB = board.getBlackPieces()->begin(); itB != board.getBlackPieces()->end(); itB++)
			{
				(*itB)->getValidMoves(&board);
			}
		}

		toSquare = _locationSquareMap.at(Location((File)toFile, toRank));
		if (fromSquare->getCurrentMan()->getColor() == currentTurn)
		{
			if (currentTurn == ManColor::WHITE)
			{
				if (std::count_if(board.getWhitePieces()->begin(), board.getWhitePieces()->end(), hasToCapture) && !fromSquare->getCurrentMan()->getHasCaptureMove())
				{
					std::cout << "Not a valid move. Another piece has to capture." << std::endl;
					continue;
				}
			}
			else
			{
				if (std::count_if(board.getBlackPieces()->begin(), board.getBlackPieces()->end(), hasToCapture) && !fromSquare->getCurrentMan()->getHasCaptureMove())
				{
					std::cout << "Not a valid move. Another piece has to capture." << std::endl;
					continue;
				}
			}
			if (!fromSquare->getCurrentMan()->getIsKing())
			{
				std::vector<Location> possibleLocations = fromSquare->getCurrentMan()->getValidMoves(&board);
				if (possibleLocations.size() != 0)
				{
					bool moved = false;

					//prints the list of available moves for selected pieces. Used for debugging purposes.
					/*for (std::vector<Location>::iterator it = possibleLocations.begin(); it != possibleLocations.end(); it++)
					{
						std::cout << *it << std::endl;
					}*/

					if (std::count(possibleLocations.begin(), possibleLocations.end(), toSquare->getLocation()))
					{
						//right forward capture
						if (toSquare->getLocation().getRank() == fromSquare->getLocation().getRank() + 2 && int(toSquare->getLocation().getFile()) == int(fromSquare->getLocation().getFile()) + 2)
						{
							Square* preSquare = _locationSquareMap[LocationFactory::createLocation(toSquare->getLocation(), -1, -1)];
							if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE) {
								std::vector <Man*>* pieces = board.getWhitePieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							else
							{
								std::vector <Man*>* pieces = board.getBlackPieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							delete preSquare->getCurrentMan();
							preSquare->reset();
						}
						//left forward capture
						else if (toSquare->getLocation().getRank() == fromSquare->getLocation().getRank() + 2 && int(toSquare->getLocation().getFile()) == int(fromSquare->getLocation().getFile()) - 2)
						{
							Square* preSquare = _locationSquareMap[LocationFactory::createLocation(toSquare->getLocation(), 1, -1)];

							if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE) {
								std::vector <Man*>* pieces = board.getWhitePieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							else
							{
								std::vector <Man*>* pieces = board.getBlackPieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							delete preSquare->getCurrentMan();
							preSquare->reset();
						}
						//right backwards capture
						else if (toSquare->getLocation().getRank() == fromSquare->getLocation().getRank() - 2 && int(toSquare->getLocation().getFile()) == int(fromSquare->getLocation().getFile()) + 2)
						{
							Square* preSquare = _locationSquareMap[LocationFactory::createLocation(toSquare->getLocation(), -1, 1)];
							if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE) {
								std::vector <Man*>* pieces = board.getWhitePieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							else
							{
								std::vector <Man*>* pieces = board.getBlackPieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}

							delete preSquare->getCurrentMan();
							preSquare->reset();
						}
						//left backwards capture
						else if (toSquare->getLocation().getRank() == fromSquare->getLocation().getRank() - 2 && int(toSquare->getLocation().getFile()) == int(fromSquare->getLocation().getFile()) - 2)
						{
							Square* preSquare = _locationSquareMap[LocationFactory::createLocation(toSquare->getLocation(), 1, 1)];
							if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE) {
								std::vector <Man*>* pieces = board.getWhitePieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							else
							{
								std::vector <Man*>* pieces = board.getBlackPieces();
								pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
							}
							delete preSquare->getCurrentMan();
							preSquare->reset();
						}

						fromSquare->getCurrentMan()->makeMove(toSquare);
						std::cout << "Made a move." << std::endl;
						moved = true;

						//set kings if piece reaches certain rank
						if (toSquare->getCurrentMan()->getColor() == ManColor::WHITE && toSquare->getLocation().getRank() == 8)
						{
							toSquare->getCurrentMan()->setIsKing();
						}
						else if (toSquare->getCurrentMan()->getColor() == ManColor::BLACK && toSquare->getLocation().getRank() == 1)
						{
							toSquare->getCurrentMan()->setIsKing();
						}


						for (std::vector<Man*>::iterator itW = board.getWhitePieces()->begin(); itW != board.getWhitePieces()->end(); itW++)
						{
							(*itW)->setHasCaptureMove(false);
						}
						for (std::vector<Man*>::iterator itB = board.getBlackPieces()->begin(); itB != board.getBlackPieces()->end(); itB++)
						{
							(*itB)->setHasCaptureMove(false);
						}
						if (board.getBlackPieces()->size() == 0)
						{
							board.printBoard();
							std::cout << "\nEnd of the game: white have won." << std::endl;
							running = false;
							break;
						}
						else if (board.getWhitePieces()->size() == 0)
						{
							board.printBoard();
							std::cout << "\nEnd of the game: black have won." << std::endl;
							running = false;
							break;
						}

					}
					if (!moved)
					{
						if (fromSquare->getCurrentMan()->getHasCaptureMove()) std::cout << "Not a valid move, you have to capture" << std::endl;
						else std::cout << "Not a valid move." << std::endl; //might need add target square is occupied
					}
					else
					{
						board.printBoard();

						if (std::abs(toSquare->getLocation().getRank() - fromSquare->getLocation().getRank()) == 2) // cheking for multitake
						{
							std::vector<Location> _possibleLocations_ = toSquare->getCurrentMan()->getValidMoves(&board);
							if (std::count(_possibleLocations_.begin(), _possibleLocations_.end(), LocationFactory::createLocation(toSquare->getLocation(), 2, -2)) ||
								std::count(_possibleLocations_.begin(), _possibleLocations_.end(), LocationFactory::createLocation(toSquare->getLocation(), -2, -2)) ||
								std::count(_possibleLocations_.begin(), _possibleLocations_.end(), LocationFactory::createLocation(toSquare->getLocation(), 2, 2)) ||
								std::count(_possibleLocations_.begin(), _possibleLocations_.end(), LocationFactory::createLocation(toSquare->getLocation(), -2, 2))
								)
							{
								std::cout << "You have at least one more capture move from this square." << std::endl;
								continue;
							}
						}

						if (currentTurn == ManColor::WHITE)
						{
							currentTurn = ManColor::BLACK;
						}
						else
						{
							currentTurn = ManColor::WHITE;
						}
					}
				}
				else
				{
					std::cout << "No valid moves for this piece." << std::endl;
				}
			}
			else //if is a king
			{
				std::vector<Location> possibleLocations = fromSquare->getCurrentMan()->getValidMoves(&board);
				if (possibleLocations.size() != 0)
				{
					bool moved = false;
					
					//prints the list of available moves for selected pieces. Used for debugging purposes.
					/*for (std::vector<Location>::iterator it = possibleLocations.begin(); it != possibleLocations.end(); it++)
					{
						std::cout << *it << std::endl;
					}*/
					
					if (std::count(possibleLocations.begin(), possibleLocations.end(), toSquare->getLocation()))
					{
						//check capture up right 
						if (int(fromSquare->getLocation().getFile()) < int(toSquare->getLocation().getFile()) && fromSquare->getLocation().getRank() < toSquare->getLocation().getRank())
						{
							for (int f = int(fromSquare->getLocation().getFile()) + 1, r = fromSquare->getLocation().getRank() + 1; f <= 8 && r <= 8; f++, r++)
							{
								Square* preSquare = _locationSquareMap[Location((File)f, r)];
								if (preSquare->getIsOccupied())
								{
									std::vector<Man*>* pieces;
									if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE)
									{
										pieces = board.getWhitePieces();
									}
									else
									{
										pieces = board.getBlackPieces();
									}
									pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
									delete preSquare->getCurrentMan();
									preSquare->reset();
									break;
								}
							}
						}

						//check capture up left 
						if (int(fromSquare->getLocation().getFile()) > int(toSquare->getLocation().getFile()) && fromSquare->getLocation().getRank() < toSquare->getLocation().getRank())
						{
							for (int f = int(fromSquare->getLocation().getFile()) - 1, r = fromSquare->getLocation().getRank() + 1; f >= 1 && r <= 8; f--, r++)
							{
								Square* preSquare = _locationSquareMap[Location((File)f, r)];
								if (preSquare->getIsOccupied())
								{
									std::vector<Man*>* pieces;
									if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE)
									{
										pieces = board.getWhitePieces();
									}
									else
									{
										pieces = board.getBlackPieces();
									}
									pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
									delete preSquare->getCurrentMan();
									preSquare->reset();
									break;
								}
							}
						}

						//check capture down right 
						if (int(fromSquare->getLocation().getFile()) < int(toSquare->getLocation().getFile()) && fromSquare->getLocation().getRank() > toSquare->getLocation().getRank())
						{
							for (int f = int(fromSquare->getLocation().getFile()) + 1, r = fromSquare->getLocation().getRank() - 1; f <= 8 && r >= 1; f++, r--)
							{
								Square* preSquare = _locationSquareMap[Location((File)f, r)];
								if (preSquare->getIsOccupied())
								{
									std::vector<Man*>* pieces;
									if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE)
									{
										pieces = board.getWhitePieces();
									}
									else
									{
										pieces = board.getBlackPieces();
									}
									pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
									delete preSquare->getCurrentMan();
									preSquare->reset();
									break;
								}
							}
						}

						//check capture down left 
						if (int(fromSquare->getLocation().getFile()) > int(toSquare->getLocation().getFile()) && fromSquare->getLocation().getRank() > toSquare->getLocation().getRank())
						{
							for (int f = int(fromSquare->getLocation().getFile()) - 1, r = fromSquare->getLocation().getRank() - 1; f >= 1 && r >= 1; f--, r--)
							{
								Square* preSquare = _locationSquareMap[Location((File)f, r)];
								if (preSquare->getIsOccupied())
								{
									std::vector<Man*>* pieces;
									if (preSquare->getCurrentMan()->getColor() == ManColor::WHITE)
									{
										pieces = board.getWhitePieces();
									}
									else
									{
										pieces = board.getBlackPieces();
									}
									pieces->erase(std::remove(pieces->begin(), pieces->end(), preSquare->getCurrentMan()), pieces->end());
									delete preSquare->getCurrentMan();
									preSquare->reset();
									break;
								}
							}
						}
						fromSquare->getCurrentMan()->makeMove(toSquare);
						std::cout << "Made a move" << std::endl;
						moved = true;
						board.printBoard();

						for (std::vector<Man*>::iterator itW = board.getWhitePieces()->begin(); itW != board.getWhitePieces()->end(); itW++)
						{
							(*itW)->setHasCaptureMove(false);
						}
						for (std::vector<Man*>::iterator itB = board.getBlackPieces()->begin(); itB != board.getBlackPieces()->end(); itB++)
						{
							(*itB)->setHasCaptureMove(false);
						}
						if (board.getBlackPieces()->size() == 0)
						{
							board.printBoard();
							std::cout << "\nEnd of the game: white have won." << std::endl;
							running = false;
							break;
						}
						else if (board.getWhitePieces()->size() == 0)
						{
							board.printBoard();
							std::cout << "\nEnd of the game: black have won." << std::endl;
							running = false;
							break;
						}

						
					}
					if (!moved)
					{
						if (fromSquare->getCurrentMan()->getHasCaptureMove()) std::cout << "Not a valid move, you have to capture" << std::endl;
						else std::cout << "Not a valid move." << std::endl;
					}
					else
					{
						toSquare->getCurrentMan()->getValidMoves(&board);
						if (toSquare->getCurrentMan()->getHasCaptureMove())
						{
							std::cout << "You at least one move capture move from this square." << std::endl;
							continue;
						}

						if (currentTurn == ManColor::WHITE)
						{
							currentTurn = ManColor::BLACK;
						}
						else
						{
							currentTurn = ManColor::WHITE;
						}
					}

				}
				else
				{
					std::cout << "No valid moves for this piece." << std::endl;
				}
			}
		}
		else
		{
			if (currentTurn == ManColor::WHITE) std::cout << "Not your move, it's white's move." << std::endl;
			else std::cout << "Not your move, it's black's move." << std::endl;
		}
	}
}

//additional function. Was used for erasing from the vector
bool hasToCapture(Man* man)
{
	return man->getHasCaptureMove();
}