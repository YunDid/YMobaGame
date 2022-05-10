// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "YMobaGameHUD.generated.h"

class UUI_MainScreen;
/**
 *
 */
UCLASS()
class AYMobaGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AYMobaGameHUD();

	virtual void BeginPlay() override;

private:
	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
};
