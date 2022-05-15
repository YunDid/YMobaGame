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
	const TArray<FCharacterTable*>* FCharaterTable_Cache_Ins = GetFCharaterTable_Cache();

	for (auto& FCharacterTable_Ins : *FCharaterTable_Cache_Ins) {
		if (CharaterID == FCharacterTable_Ins->CharacterID) {
			return FCharacterTable_Ins;
		}
	}

	return nullptr;
}

const TArray<FCharacterTable*>* AYMobaGameState::GetFCharaterTable_Cache()
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

const FCharacterAttribute* AYMobaGameState::GetFCharaterAttributeByID(const int32& CharaterID)
{
	//通过单例获取角色配置表缓存
	const TArray<FCharacterAttribute*>* FCharaterAttribute_Cache_Ins = GetFCharaterAttribute_Cache();

	for (auto& FCharacterAttribute_Ins : *FCharaterAttribute_Cache_Ins) {
		if (CharaterID == FCharacterAttribute_Ins->CharacterID) {
			return FCharacterAttribute_Ins;
		}
	}

	return nullptr;
}

const TArray<FCharacterAttribute*>* AYMobaGameState::GetFCharaterAttribute_Cache()
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

	DOREPLIFETIME(AYMobaGameState, PlayerLocation);
}

const TArray<FPlayerLocation>& AYMobaGameState::GetPlayerLocations()
{
	return PlayerLocation;
}
