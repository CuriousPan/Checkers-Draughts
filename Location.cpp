#include "Location.h"

Location::Location(File file, int rank) : file(file), rank(rank)
{
	//ctor
}

Location::~Location()
{
	//dtorS
}

Location::Location(const Location& location) 
{
	this->file = location.file;
	this->rank = location.rank;
}

//not sure if it's needed, so let it be commented
//Location& Location::operator=(const Location& location)
//{
//	file = location.file;
//	rank = location.rank;
//	return *this;
//}

File Location::getFile() const 
{
	return file;
}

int Location::getRank() const
{
	return rank;
}

bool operator==(const Location& location1, const Location& location2)
{
	return location1.file == location2.file && location1.rank == location2.rank;
}

bool operator!=(const Location& location1, const Location& location2)
{
	return location1.file != location2.file || location1.rank != location2.rank;
}

bool operator<(const Location& location1, const Location& location2)
{
	if (location1.rank == location2.rank)
	{
		return int(location1.file) < int(location2.file);
	}
	return location1.rank < location2.rank;
}

std::ostream& operator<<(std::ostream& ostream, const Location& location)
{
	std::string fileStr;
	switch (location.file)
	{
	case File::A: fileStr = "A"; break;
	case File::B: fileStr = "B"; break;
	case File::C: fileStr = "C"; break;
	case File::D: fileStr = "D"; break;
	case File::E: fileStr = "E"; break;
	case File::F: fileStr = "F"; break;
	case File::G: fileStr = "G"; break;
	case File::H: fileStr = "H"; break;
	default: fileStr = "FILE"; break;
	}
	return std::cout << "Location {file=" << fileStr << ", rank=" << location.rank << "}";
}