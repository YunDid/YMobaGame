// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/CharacterTable.h"
#include "GameFramework/GameStateBase.h"
#include "MobaGameState.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API AMobaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMobaGameState();

	//获取当前使用的配置表.
	const FCharacterTable* GetFCharaterTableByID(const int64& CharaterID);

	//获取所有配置表缓存.
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache();
protected:
	//获取含所有 FCharacterTable 的 UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_Character;
private:
	//存储游戏下的所有角色配置表 FCharacterTable.
	TArray<FCharacterTable*> FCharaterTable_Cache;
};
