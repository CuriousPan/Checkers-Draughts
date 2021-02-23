#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
#include <string>

enum class File
{
	A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8
};

class Location
{
	File file;
	int rank;

public:		
	Location(File file, int rank);
	Location(const Location& location);
	~Location();
	File getFile() const;
	int getRank() const;
	friend std::ostream& operator<<(std::ostream& ostream, const Location& location);
	friend bool operator<(const Location& location1, const Location& location2);
	friend bool operator==(const Location& location1, const Location& location2);
	friend bool operator!=(const Location& location1, const Location& location2);
};



#endif
