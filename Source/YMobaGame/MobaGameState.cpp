// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaGameState.h"

AMobaGameState::AMobaGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> UDataTable_Character_Ins(TEXT("/Game/Tables/CharacterTable"));
	UDataTable_Character = UDataTable_Character_Ins.Object;
}

const FCharacterTable* AMobaGameState::GetFCharaterTableByID(const int64& CharaterID)
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

const TArray<FCharacterTable*>* AMobaGameState::GetFCharaterTable_Cache()
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