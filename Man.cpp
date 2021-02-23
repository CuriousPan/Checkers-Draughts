#include "Man.h"
#include "LocationFactory.h"

Man::Man(ManColor color)
{
	this->color = color;
	this->isKing = false;
	this->currentSquare = nullptr;
	this->hasCaptureMove = false;
	this->hasKingRightUpMove = false;
	this->hasKingLeftUpMove = false;
	this->hasKingRightDownMove = false;
	this->hasKingLeftDownMove = false;
}

Man::~Man()
{
	//dtor
}

ManColor Man::getColor()
{
	return color;
}

std::vector<Location> Man::getValidMoves(Board* board)
{
	std::vector<Location> moveCandidates{};
	std::map<Location, Square*> squareMap = board->getLocationSquareMap();

	if (!isKing)
	{
		Location currentLocation = currentSquare->getLocation();
		if (color == ManColor::WHITE)
		{
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 2, 2)); //capture forward right
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -2, 2)); //capture forward left
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 2, -2)); //capture backwards right
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -2, -2)); //capture backwards left
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 1, 1)); //move one right forward
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -1, 1)); //move one left forward
		}
		else
		{
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 2, -2)); //capture forward right
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -2, -2)); //capture forward left
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 2, 2)); //capture backwards right
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -2, 2)); //capture backwards left
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, 1, -1)); //move one right forward (remain the perspective) (or no ;) )
			moveCandidates.push_back(LocationFactory::createLocation(currentLocation, -1, -1)); //move one left forward
		}
		for (std::vector<Location>::iterator itr = moveCandidates.begin(); itr != moveCandidates.end(); )
		{
			//remove squares outside the board
			if (!squareMap.count(*itr))
			{
				itr = moveCandidates.erase(itr);
				continue;
			}

			//remove occupied squares
			if (squareMap[*itr]->getIsOccupied())
			{
				itr = moveCandidates.erase(itr);
				continue;
			}

			//remove non-capture moves if it's possible to make a capture move (for this piece)
			if (int(itr->getFile()) == int(currentLocation.getFile()) + 1 || int(itr->getFile()) == int(currentLocation.getFile()) - 1)
			{
				if (std::count(moveCandidates.begin(), moveCandidates.end(), LocationFactory::createLocation(currentLocation, 2, -2)) ||
					std::count(moveCandidates.begin(), moveCandidates.end(), LocationFactory::createLocation(currentLocation, -2, -2))||
					std::count(moveCandidates.begin(), moveCandidates.end(), LocationFactory::createLocation(currentLocation, 2, 2)) ||
					std::count(moveCandidates.begin(), moveCandidates.end(), LocationFactory::createLocation(currentLocation, -2, 2)))
				{
					itr = moveCandidates.erase(itr);
					continue;
				}

				else
				{
					++itr;
					continue;
				}
			}

			//capture forward right 
			if (int(itr->getFile()) == int(currentLocation.getFile()) + 2 && itr->getRank() == currentLocation.getRank() + 2)
			{
				Square* preSquare = squareMap[LocationFactory::createLocation(currentLocation, 1, 1)];

				if ((preSquare->getIsOccupied() && preSquare->getCurrentMan()->getColor() == color) ||
					!(preSquare->getIsOccupied()))
				{
					itr = moveCandidates.erase(itr);
					continue;
				}
				else
				{
					hasCaptureMove = true;
					++itr;
					continue;
				}
			}

			//capture forward left 
			if (int(itr->getFile()) == int(currentLocation.getFile()) - 2 && itr->getRank() == currentLocation.getRank() + 2)
			{
				Square* preSquare = squareMap[LocationFactory::createLocation(currentLocation, -1, 1)];
				if ((preSquare->getIsOccupied() && preSquare->getCurrentMan()->getColor() == color) ||
					!(preSquare->getIsOccupied()))
				{
					itr = moveCandidates.erase(itr);
					continue;
				}
				else
				{
					hasCaptureMove = true;
					++itr;
					continue;
				}
			}

			//capture backwards right  
			if (int(itr->getFile()) == int(currentLocation.getFile()) + 2 && itr->getRank() == currentLocation.getRank() - 2)
			{
				Square* preSquare = squareMap[LocationFactory::createLocation(currentLocation, 1, -1)];

				if ((preSquare->getIsOccupied() && preSquare->getCurrentMan()->getColor() == color) ||
					!(preSquare->getIsOccupied()))
				{
					itr = moveCandidates.erase(itr);
					continue;
				}
				else
				{
					hasCaptureMove = true;
					++itr;
					continue;
				}
			}

			//capture backwards left 
			if (int(itr->getFile()) == int(currentLocation.getFile()) - 2 && itr->getRank() == currentLocation.getRank() - 2)
			{
				Square* preSquare = squareMap[LocationFactory::createLocation(currentLocation, -1, -1)];

				if ((preSquare->getIsOccupied() && preSquare->getCurrentMan()->getColor() == color) ||
					!(preSquare->getIsOccupied()))
				{
					itr = moveCandidates.erase(itr);
					continue;
				}
				else
				{
					hasCaptureMove = true;
					++itr;
					continue;
				}
			}
			else
			{
				++itr;
				//continue; 
			}
		}
	}
	else // if isKing
	{
		//right down
		std::vector<Location> tmpMoveCandidatesRd;
		for (int f = int(currentSquare->getLocation().getFile()) + 1, r = currentSquare->getLocation().getRank() - 1; f <= 8 && r >= 1; f++, r--)
		{
			Location locationToCheck((File)f, r);
			if (!squareMap[locationToCheck]->getIsOccupied())
			{
				tmpMoveCandidatesRd.push_back(locationToCheck);
				continue;
			}
			else // if occupied
			{
				if (squareMap[locationToCheck]->getIsOccupied() && squareMap[locationToCheck]->getCurrentMan()->getColor() != color && !squareMap[Location((File)(f + 1), r - 1)]->getIsOccupied())
				{
					for (int f_ = int(currentSquare->getLocation().getFile()) + 1, r_ = currentSquare->getLocation().getRank() - 1; f_ <= (f - 1) && r_ >= (r + 1); f_++, r_--)//removing before the occupied square
					{

					}
					Location afterPiece((File)(f + 1), r - 1);
					tmpMoveCandidatesRd.push_back(afterPiece);
					hasKingRightDownMove = true;

					for (int _f = f + 2, _r = r - 2; _f <= 8 && _r >= 1; _f++, _r--) //STOPED here
					{
						Location otherLocationToCheck((File)_f, _r);
						if (!squareMap[otherLocationToCheck]->getIsOccupied())
						{
							tmpMoveCandidatesRd.push_back(otherLocationToCheck);
						}
						else break;
					}
					int f_ = int(currentSquare->getLocation().getFile()) + 1;
					int	r_ = currentSquare->getLocation().getRank() - 1;
					for (std::vector<Location>::iterator it = tmpMoveCandidatesRd.begin(); *it != Location((File)(f + 1), r - 1);)
					{
						it = tmpMoveCandidatesRd.erase(it);
					}
					break;
				}
				break;
			}
		}



		//left down 
		std::vector<Location> tmpMoveCandidatesLd;
		for (int f = int(currentSquare->getLocation().getFile()) - 1, r = currentSquare->getLocation().getRank() - 1; f >= 1 && r >= 1; f--, r--)
		{
			Location locationToCheck((File)f, r);
			if (!squareMap[locationToCheck]->getIsOccupied())
			{
				tmpMoveCandidatesLd.push_back(locationToCheck);
				continue;
			}
			else // if occupied
			{
				if (squareMap[locationToCheck]->getIsOccupied() && squareMap[locationToCheck]->getCurrentMan()->getColor() != color && !squareMap[Location((File)(f - 1), r - 1)]->getIsOccupied())
				{
					for (int f_ = int(currentSquare->getLocation().getFile()) - 1, r_ = currentSquare->getLocation().getRank() - 1; f_ >= (f - 1) && r_ >= (r - 1); f_--, r_--)//removing before the occupied square
					{

					}
					Location afterPiece((File)(f - 1), r - 1);
					tmpMoveCandidatesLd.push_back(afterPiece);
					hasKingLeftDownMove = true;

					for (int _f = f - 2, _r = r - 2; _f >= 1 && _r >= 1; _f--, _r--) //STOPED here
					{
						Location otherLocationToCheck((File)_f, _r);
						if (!squareMap[otherLocationToCheck]->getIsOccupied())
						{
							tmpMoveCandidatesLd.push_back(otherLocationToCheck);
						}
						else break;
					}
					int f_ = int(currentSquare->getLocation().getFile()) - 1;
					int	r_ = currentSquare->getLocation().getRank() - 1;
					for (std::vector<Location>::iterator it = tmpMoveCandidatesLd.begin(); *it != Location((File)(f - 1), r - 1);)
					{
						it = tmpMoveCandidatesLd.erase(it);
					}
					break;
				}
				break;
			}
		}


		//up right
		std::vector<Location> tmpMoveCandidatesUr;
		for (int f = int(currentSquare->getLocation().getFile()) + 1, r = currentSquare->getLocation().getRank() + 1; f <= 8 && r <= 8; f++, r++)
		{
			Location locationToCheck((File)f, r);
			if (!squareMap[locationToCheck]->getIsOccupied())
			{
				tmpMoveCandidatesUr.push_back(locationToCheck);
				continue;
			}
			else // if occupied
			{
				if (squareMap[locationToCheck]->getIsOccupied() && squareMap[locationToCheck]->getCurrentMan()->getColor() != color && !squareMap[Location((File)(f + 1), r + 1)]->getIsOccupied())
				{
					for (int f_ = int(currentSquare->getLocation().getFile()) + 1, r_ = currentSquare->getLocation().getRank() + 1; f_ <= (f + 1) && r_ <= (r + 1); f_++, r_++)//removing before the occupied square
					{

					}
					Location afterPiece((File)(f + 1), r + 1);
					tmpMoveCandidatesUr.push_back(afterPiece);
					hasKingRightUpMove = true;

					for (int _f = f + 2, _r = r + 2; _f <= 8 && _r <= 8; _f++, _r++) //STOPED here
					{
						Location otherLocationToCheck((File)_f, _r);
						if (!squareMap[otherLocationToCheck]->getIsOccupied())
						{
							tmpMoveCandidatesUr.push_back(otherLocationToCheck);
						}
						else break;
					}
					int f_ = int(currentSquare->getLocation().getFile()) + 1;
					int	r_ = currentSquare->getLocation().getRank() + 1;
					for (std::vector<Location>::iterator it = tmpMoveCandidatesUr.begin(); *it != Location((File)(f + 1), r + 1);)
					{
						it = tmpMoveCandidatesUr.erase(it);
					}
					break;
				}
				break;
			}
		}


		//up left
		std::vector<Location> tmpMoveCandidatesUl;
		for (int f = int(currentSquare->getLocation().getFile()) - 1, r = currentSquare->getLocation().getRank() + 1; f >= 1 && r <= 8; f--, r++)
		{
			Location locationToCheck((File)f, r);
			if (!squareMap[locationToCheck]->getIsOccupied())
			{
				tmpMoveCandidatesUl.push_back(locationToCheck);
				continue;
			}
			else // if occupied
			{
				if (squareMap[locationToCheck]->getIsOccupied() && squareMap[locationToCheck]->getCurrentMan()->getColor() != color && !squareMap[Location((File)(f - 1), r + 1)]->getIsOccupied())
				{
					for (int f_ = int(currentSquare->getLocation().getFile()) - 1, r_ = currentSquare->getLocation().getRank() + 1; f_ >= (f - 1) && r_ <= (r + 1); f_--, r_++)//removing before the occupied square
					{

					}
					Location afterPiece((File)(f - 1), r + 1);
					tmpMoveCandidatesUl.push_back(afterPiece);
					hasKingLeftUpMove = true;

					for (int _f = f - 2, _r = r + 2; _f >= 1 && _r <= 8; _f--, _r++) //STOPED here
					{
						Location otherLocationToCheck((File)_f, _r);
						if (!squareMap[otherLocationToCheck]->getIsOccupied())
						{
							tmpMoveCandidatesUl.push_back(otherLocationToCheck);
						}
						else break;
					}
					int f_ = int(currentSquare->getLocation().getFile()) - 1;
					int	r_ = currentSquare->getLocation().getRank() + 1;
					for (std::vector<Location>::iterator it = tmpMoveCandidatesUl.begin(); *it != Location((File)(f - 1), r + 1);)
					{
						it = tmpMoveCandidatesUl.erase(it);
					}
					break;
				}
				break;
			}
		}

		if (hasKingRightDownMove)
		{
			hasCaptureMove = true;
			for (std::vector<Location>::iterator it = tmpMoveCandidatesRd.begin(); it != tmpMoveCandidatesRd.end(); it++)
			{
				moveCandidates.push_back(*it);
			}
		}

		if (hasKingLeftDownMove)
		{
			hasCaptureMove = true;
			for (std::vector<Location>::iterator it = tmpMoveCandidatesLd.begin(); it != tmpMoveCandidatesLd.end(); it++)
			{
				moveCandidates.push_back(*it);
			}
		}

		if (hasKingRightUpMove)
		{
			hasCaptureMove = true;
			for (std::vector<Location>::iterator it = tmpMoveCandidatesUr.begin(); it != tmpMoveCandidatesUr.end(); it++)
			{
				moveCandidates.push_back(*it);
			}
		}

		if (hasKingLeftUpMove)
		{
			hasCaptureMove = true;
			for (std::vector<Location>::iterator it = tmpMoveCandidatesUl.begin(); it != tmpMoveCandidatesUl.end(); it++)
			{
				moveCandidates.push_back(*it);
			}
		}

		if (!hasKingLeftDownMove && !hasKingRightDownMove && !hasKingLeftUpMove && !hasKingRightUpMove)
		{
			for (std::vector<Location>::iterator it = tmpMoveCandidatesRd.begin(); it != tmpMoveCandidatesRd.end(); it++)
			{
				moveCandidates.push_back(*it);
			}

			for (std::vector<Location>::iterator it = tmpMoveCandidatesRd.begin(); it != tmpMoveCandidatesRd.end(); it++)
			{
				moveCandidates.push_back(*it);
			}

			for (std::vector<Location>::iterator it = tmpMoveCandidatesUr.begin(); it != tmpMoveCandidatesUr.end(); it++)
			{
				moveCandidates.push_back(*it);
			}

			for (std::vector<Location>::iterator it = tmpMoveCandidatesUl.begin(); it != tmpMoveCandidatesUl.end(); it++)
			{
				moveCandidates.push_back(*it);
			}
		}

	}

	

	return moveCandidates;
}

Square* Man::getCurrentSquare()
{
	return currentSquare;
}

void Man::setCurrentSquare(Square* square)
{
	this->currentSquare = square;
}

bool Man::getIsKing()
{
	return isKing;
}

void Man::setIsKing()
{
	isKing = true;
}

void Man::makeMove(Square* square)
{
	currentSquare->reset();
	currentSquare = square;
	square->setCurrentMan(this);
	square->setIsOccupied();
}

std::ostream& operator<<(std::ostream& ostream, const Man& man)
{
	std::string colorStr;
	switch (man.color)
	{
	case ManColor::BLACK: colorStr = "BLACK"; break;
	case ManColor::WHITE: colorStr = "WHITE"; break;
	default: colorStr = "COLOR";
	}
	return std::cout << "Man { color=" << colorStr << " } " << std::endl;
}

 bool operator== (const Man& man1, const Man& man2)
{
	return man1.color == man2.color && man1.currentSquare == man2.currentSquare;
}

 bool Man::getHasCaptureMove()
 {
	 return hasCaptureMove;
}

 void Man::setHasCaptureMove(bool state)
 {
	 hasCaptureMove = state;
 }
