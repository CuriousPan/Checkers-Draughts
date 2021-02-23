#ifndef BOARD_H
#define BOARD_H
#include "Square.h"
#include <vector>
#include <map>
class Man;


//class Square;
typedef std::vector<Square*> Row;

class Board
{
	const int BOARD_SIZE = 8;
	std::vector<Row> squares;
	std::map<Location, Square*> locationSquareMap; 
	std::vector<Man*> whitePieces;
	std::vector<Man*> blackPieces;

public:
	Board();
	~Board();
	//Board(Board& board);
	void printBoard();
	std::map<Location, Square*> getLocationSquareMap();
	std::vector<Man*>* getWhitePieces();
	std::vector<Man*>* getBlackPieces();
};

#endif