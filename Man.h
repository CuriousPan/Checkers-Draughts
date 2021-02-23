#ifndef MAN_H
#define MAN_H
#include <iostream>
#include <string>
#include <vector>


#include "Board.h"

//class Board;
//class Location;
//class Square;

enum class ManColor
{
	WHITE, BLACK
};

class Man
{
	ManColor color;
	Square* currentSquare;
	bool isKing;
	bool hasCaptureMove;
	bool hasKingRightUpMove;
	bool hasKingLeftUpMove;
	bool hasKingRightDownMove;
	bool hasKingLeftDownMove;

public:
	Man(ManColor color);
	~Man();
	//Man(Man& man);
	Square* getCurrentSquare();
	void setCurrentSquare(Square* square);
	bool getIsKing();
	void setIsKing();
	ManColor getColor();
	void makeMove(Square* square);
	std::vector<Location> getValidMoves(Board* board);
	bool getHasCaptureMove();
	void setHasCaptureMove(bool state);
	friend std::ostream& operator<<(std::ostream& ostream, const Man& man);
	friend bool operator== (const Man &man1, const Man &man2);
};

#endif