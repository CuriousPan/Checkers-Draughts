#ifndef LOCATION_FACTORY_H
#define LOCATION_FACTORY_H
#include "Location.h"

class LocationFactory
{
	LocationFactory();
	~LocationFactory();

public:
	static Location createLocation(Location currentLocation, int fileOffset, int rankOffset);
};

#endif