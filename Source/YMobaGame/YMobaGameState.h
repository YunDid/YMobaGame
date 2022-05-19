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

	//��ȡID��Ӧ�Ľ�ɫ���ñ�.
	const FCharacterTable* GetFCharaterTableByID(const int32& CharaterID);

	//��ȡ���н�ɫ���ñ���.
	const TArray<FCharacterTable*>* GetFCharaterTableCache_Template();

	//��ȡID��Ӧ�Ľ�ɫ���Ա�.
	const FCharacterAttribute* GetFCharaterAttributeByID_Template(const int32& CharaterID);

	//��ȡ���н�ɫ���Ա���.
	const TArray<FCharacterAttribute*>* GetFCharaterAttributeCache_Template();

	//��ȡ���� AllPlayersLocation.
	const TArray<FPlayerLocation>& GetPlayerLocations();

	//��ȡ <��ң���ǰ���ƵĽ�ɫ����> Map��.
	const TMap<int64, FCharacterAttribute>* GetCharacterAttributes_Map();

	//ͨ�����ID��ȡ��Ӧ��ɫ�����Ա�.
	FCharacterAttribute* GetCharacterAttributeByID(int64 PlayerID);

	//ͨ��ID��ȡ��ɫλ��.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetPlayerLocationByID(int64 InPlayerID, FVector& InPlayerLocation);

	//�����Ҷ�Ӧ��ɫ������.
	void AddPlayerAttribute(int64 PlayerID, int32 CharacterID);

	//������λ������ͬ������.
	void AddPlayerLocation(int64 PlayerID, const FVector& InPlayerLocation);

	//���� AllPlayersLocation ����Ӧ��ҵ�λ��.
	void UpdateCharacterAILocation(int64 PlayerID, const FVector& InPlayerLocation);

	//��ȡ��ǰ���ʹ�õĽ�ɫ����ID.
	int32 GetCurrentCharacterID(int64 PlayerID);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	//��ȡ������ FCharacterTable �� UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_Character;

	//��ȡ������ FCharacterAttribute �� UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_CharacterAttribute;
private:
	//�洢��Ϸ�µ����н�ɫ���ñ� FCharacterTable.
	TArray<FCharacterTable*> FCharaterTable_Cache;

	//�洢��Ϸ�µ����н�ɫ���Ա� FCharacterTable.
	TArray<FCharacterAttribute*> FCharaterAttribute_Cache;

	UPROPERTY(Replicated)
	TArray<FPlayerLocation> AllPlayersLocation;

	UPROPERTY()
	TMap<int64, FCharacterAttribute> CharacterAttributes_Map;
};
         