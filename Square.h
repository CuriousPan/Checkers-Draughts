#ifndef SQUARE_H
#define SQUARE_H
#include "Location.h"
#include <iostream>
class Man; //still some question about this

enum class SquareColor
{
	WHITE, BLACK
};


class Square
{
	Location location;
	SquareColor color;
	bool isOccupied;
	Man* currentMan;

public:
	Square(Location location, SquareColor color);
	~Square();
	//Square(Square& square);

	Location getLocation();
	SquareColor getColor();
	bool getIsOccupied();
	void setIsOccupied();	
	void reset();
	void setCurrentMan(Man* man);
	Man* getCurrentMan();
	friend std::ostream& operator<<(std::ostream& ostream, const Square& square);
	friend bool operator==(const Square& square1, const Square& square2);
};

#endif