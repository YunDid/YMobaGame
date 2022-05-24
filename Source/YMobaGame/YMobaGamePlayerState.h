// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerComponent/PlayerDataComponent.h"
#include "YMobaGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API AYMobaGamePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AYMobaGamePlayerState();

	const UPlayerDataComponent* GetPlayerData();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	UPlayerDataComponent* PlayerData;

};
