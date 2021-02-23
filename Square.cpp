#include "Square.h"

Square::Square(Location location, SquareColor color) : location(location), color(color)
{
	//TODO isOccupied
	this->location = location;
	this->color = color;
	this->currentMan = nullptr;
	this->isOccupied = false;
	//ctor
}

Square::~Square()
{

	//dtor
}

void Square::setCurrentMan(Man* man)
{
	this->currentMan = man;
}

Man* Square::getCurrentMan()
{
	return currentMan;
}

Location Square::getLocation()
{
	return location;
}

SquareColor Square::getColor()
{
	return color;
}

bool Square::getIsOccupied()
{
	return isOccupied;
}

void Square::setIsOccupied()
{
	isOccupied = true;
}

void Square::reset()
{
	isOccupied = false;
	currentMan = nullptr;
}

bool operator==(const Square& square1, const Square& square2)
{
	return square1.color == square2.color && square1.location == square2.location;
}

std::ostream& operator<<(std::ostream& ostream, const Square& square)
{
	std::string colorStr;
	switch (square.color)
	{
	case SquareColor::WHITE: colorStr = "WHITE"; break;
	case SquareColor::BLACK: colorStr = "BLACK"; break;
	default:colorStr = "COLOR";	break;
	}
	return std::cout << "Square { squareColor=" << colorStr << ", location=" << square.location << ", isOccupied=" << square.isOccupied << " }" << std::endl;
}
