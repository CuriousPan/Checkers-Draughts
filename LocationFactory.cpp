#include "LocationFactory.h"

LocationFactory::LocationFactory()
{

}

LocationFactory::~LocationFactory()
{
	
}

Location LocationFactory::createLocation(Location currentLocation, int fileOffSet, int rankOffSet)
{
	return Location((File)(int(currentLocation.getFile()) + fileOffSet), currentLocation.getRank() + rankOffSet);
}