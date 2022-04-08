// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/CharacterTable.h"
#include "GameFramework/GameStateBase.h"
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

	//��ȡ��ǰʹ�õ����ñ�.
	const FCharacterTable* GetFCharaterTableByID(const int64& CharaterID);

	//��ȡ�������ñ���.
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache();
protected:
	//��ȡ������ FCharacterTable �� UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_Character;
private:
	//�洢��Ϸ�µ����н�ɫ���ñ� FCharacterTable.
	TArray<FCharacterTable*> FCharaterTable_Cache;
};
