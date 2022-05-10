// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameHUD.h"
#include "UI/Game/UI_MainScreen.h"

AYMobaGameHUD::AYMobaGameHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> HallMain_BPClass(TEXT("/Game/UI/Game/MainScreen_BP"));
	MainScreenClass = HallMain_BPClass.Class;
}

void AYMobaGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainScreenClass)
	{
		MainScreen = CreateWidget<UUI_MainScreen>(GetWorld(), MainScreenClass);
		MainScreen->AddToPlayerScreen(2);
	}
}
