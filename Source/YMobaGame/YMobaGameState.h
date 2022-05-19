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
	const TArray<FCharacterTable*>* GetFCharaterTableCache_Template();

	//获取ID对应的角色属性表.
	const FCharacterAttribute* GetFCharaterAttributeByID_Template(const int32& CharaterID);

	//获取所有角色属性表缓存.
	const TArray<FCharacterAttribute*>* GetFCharaterAttributeCache_Template();

	//获取所有 AllPlayersLocation.
	const TArray<FPlayerLocation>& GetPlayerLocations();

	//获取 <玩家，当前控制的角色属性> Map表.
	const TMap<int64, FCharacterAttribute>* GetCharacterAttributes_Map();

	//通过玩家ID获取对应角色的属性表.
	FCharacterAttribute* GetCharacterAttributeByID(int64 PlayerID);

	//通过ID获取角色位置.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetPlayerLocationByID(int64 InPlayerID, FVector& InPlayerLocation);

	//添加玩家对应角色的属性.
	void AddPlayerAttribute(int64 PlayerID, int32 CharacterID);

	//添加玩家位置用作同步缓存.
	void AddPlayerLocation(int64 PlayerID, const FVector& InPlayerLocation);

	//更新 AllPlayersLocation 中相应玩家的位置.
	void UpdateCharacterAILocation(int64 PlayerID, const FVector& InPlayerLocation);

	//获取当前玩家使用的角色配置ID.
	int32 GetCurrentCharacterID(int64 PlayerID);

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
	TArray<FPlayerLocation> AllPlayersLocation;

	UPROPERTY()
	TMap<int64, FCharacterAttribute> CharacterAttributes_Map;
};
         