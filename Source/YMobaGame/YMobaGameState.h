// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/CharacterTable.h"
#include "GameFramework/GameStateBase.h"
#include "Table/CharacterAttribute.h"
#include "Data/MisData.h"
#include "YMobaGameState.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API AYMobaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AYMobaGameState();

	//获取ID对应的角色配置表.
	const FCharacterTable* GetFCharaterTableByID(const int32& CharaterID);

	//获取所有角色配置表缓存.
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache();

	//获取ID对应的角色属性表.
	const FCharacterAttribute* GetFCharaterAttributeByID(const int32& CharaterID);

	//获取所有角色属性表缓存.
	const TArray<FCharacterAttribute*>* GetFCharaterAttribute_Cache();

	//获取所有 PlayerLocation.
	const TArray<FPlayerLocation>& GetPlayerLocations();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	//获取含所有 FCharacterTable 的 UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_Character;

	//获取含所有 FCharacterAttribute 的 UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_CharacterAttribute;
private:
	//存储游戏下的所有角色配置表 FCharacterTable.
	TArray<FCharacterTable*> FCharaterTable_Cache;

	//存储游戏下的所有角色属性表 FCharacterTable.
	TArray<FCharacterAttribute*> FCharaterAttribute_Cache;

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> PlayerLocation;
};
