#include "MisData.h"

FPlayerLocation::FPlayerLocation()
	:PlayerID(INDEX_NONE)
	, Location(FVector::ZeroVector)
{

}

FPlayerLocation::FPlayerLocation(int64 InPlayerID, const FVector& PlayerLocation)
	:PlayerID(InPlayerID)
	, Location(PlayerLocation) 
{
	
}
