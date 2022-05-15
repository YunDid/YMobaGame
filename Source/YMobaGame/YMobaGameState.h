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
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache();

	//��ȡID��Ӧ�Ľ�ɫ���Ա�.
	const FCharacterAttribute* GetFCharaterAttributeByID(const int32& CharaterID);

	//��ȡ���н�ɫ���Ա���.
	const TArray<FCharacterAttribute*>* GetFCharaterAttribute_Cache();

	//��ȡ���� PlayerLocation.
	const TArray<FPlayerLocation>& GetPlayerLocations();

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
	TArray<FPlayerLocation> PlayerLocation;
};
