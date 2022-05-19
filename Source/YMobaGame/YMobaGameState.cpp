// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameState.h"
#include "Net/UnrealNetwork.h"

AYMobaGameState::AYMobaGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> UDataTable_Character_Ins(TEXT("/Game/Tables/CharacterTable"));
	UDataTable_Character = UDataTable_Character_Ins.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> UDataTable_CharacterAttribute_Ins(TEXT("/Game/Tables/CharacterAttribute"));
	UDataTable_CharacterAttribute = UDataTable_CharacterAttribute_Ins.Object;
}

const FCharacterTable* AYMobaGameState::GetFCharaterTableByID(const int32& CharaterID)
{
	//通过单例获取角色配置表缓存
	const TArray<FCharacterTable*>* FCharaterTable_Cache_Ins = GetFCharaterTableCache_Template();

	for (auto& FCharacterTable_Ins : *FCharaterTable_Cache_Ins) {
		if (CharaterID == FCharacterTable_Ins->CharacterID) {
			return FCharacterTable_Ins;
		}
	}

	return nullptr;
}

const TArray<FCharacterTable*>* AYMobaGameState::GetFCharaterTableCache_Template()
{
	if (!FCharaterTable_Cache.Num()) {

		if (UDataTable_Character) {
			UDataTable_Character->GetAllRows(TEXT("Character Class"), FCharaterTable_Cache);
		}

	}
	else {
		//
	}

	return &FCharaterTable_Cache;
}

const FCharacterAttribute* AYMobaGameState::GetFCharaterAttributeByID_Template(const int32& CharaterID)
{
	//通过单例获取角色配置表缓存
	const TArray<FCharacterAttribute*>* FCharaterAttribute_Cache_Ins = GetFCharaterAttributeCache_Template();

	for (auto& FCharacterAttribute_Ins : *FCharaterAttribute_Cache_Ins) {
		if (CharaterID == FCharacterAttribute_Ins->CharacterID) {
			return FCharacterAttribute_Ins;
		}
	}

	return nullptr;
}

const TArray<FCharacterAttribute*>* AYMobaGameState::GetFCharaterAttributeCache_Template()
{
	if (!FCharaterAttribute_Cache.Num()) {

		if (UDataTable_CharacterAttribute) {
			UDataTable_CharacterAttribute->GetAllRows(TEXT("Character Class"), FCharaterAttribute_Cache);
		}

	}
	else {
		//
	}

	return &FCharaterAttribute_Cache;
}

void AYMobaGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AYMobaGameState, AllPlayersLocation);
}

const TArray<FPlayerLocation>& AYMobaGameState::GetPlayerLocations()
{
	return AllPlayersLocation;
}

const TMap<int64, FCharacterAttribute>* AYMobaGameState::GetCharacterAttributes_Map()
{
	return &CharacterAttributes_Map;
}

FCharacterAttribute* AYMobaGameState::GetCharacterAttributeByID(int64 PlayerID)
{
	for (auto& Pair : CharacterAttributes_Map) {
		if (Pair.Key == PlayerID) {
			return &(Pair.Value);
		}
	}

	return nullptr;
}

bool AYMobaGameState::GetPlayerLocationByID(int64 InPlayerID, FVector& InPlayerLocation)
{
	for (auto& Tmp : AllPlayersLocation)
	{
		if (Tmp.PlayerID == InPlayerID)
		{
			InPlayerLocation = Tmp.Location;
			return true;
		}
	}

	return false;
}

void AYMobaGameState::AddPlayerAttribute(int64 PlayerID, int32 CharacterID)
{
	if (!CharacterAttributes_Map.Contains(PlayerID)) {
		CharacterAttributes_Map.Add(PlayerID, *GetFCharaterAttributeByID_Template(CharacterID));
	}
}

void AYMobaGameState::AddPlayerLocation(int64 PlayerID, const FVector& InPlayerLocation)
{
	for (auto& PlayerLocation_Ins : AllPlayersLocation)
	{
		if (PlayerLocation_Ins.PlayerID == PlayerID)
		{
			return;
		}
	}

	AllPlayersLocation.Add(FPlayerLocation(PlayerID, InPlayerLocation));
}

void AYMobaGameState::UpdateCharacterAILocation(int64 PlayerID, const FVector& InPlayerLocation)
{
	for (auto& PlayerLocation_Ins : AllPlayersLocation)
	{
		if (PlayerLocation_Ins.PlayerID == PlayerID)
		{
			PlayerLocation_Ins.Location = InPlayerLocation;
			break;
		}
	}
}

int32 AYMobaGameState::GetCurrentCharacterID(int64 PlayerID)
{
	if (FCharacterAttribute* CA = GetCharacterAttributeByID(PlayerID))
	{
		return CA->CharacterID;
	}

	return INDEX_NONE;
}




