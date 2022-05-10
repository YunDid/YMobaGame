// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../YMobaGameHUD.h"
#include "UI_Base.generated.h"

UCLASS()
class UUI_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	AYMobaGameHUD* GetHUD();
};