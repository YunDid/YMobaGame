// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameHUD.h"
#include "UI/Game/UI_MainScreen.h"

AYMobaGameHUD::AYMobaGameHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_MainScreen> HallMain_BPClass(TEXT("/Game/TopDownCPP/UI/Game/MainScreen_BP"));
	MainScreenClass = HallMain_BPClass.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> UDataTable_ClientGlobalConfiguration_Ins(TEXT("/Game/Tables/ClientGlobalConfigurationTable"));
	UDataTable_ClientGlobalConfiguration = UDataTable_ClientGlobalConfiguration_Ins.Object;
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

const FGlobalConfiguration_Client* AYMobaGameHUD::GetClientGlobalConfiguration() {

	//µ¥Àý³õÊ¼»¯ ClientGlobalConfiguration
	if (!ClientGlobalConfiguration_Cache.Num()) {

		if (UDataTable_ClientGlobalConfiguration) {
			UDataTable_ClientGlobalConfiguration->GetAllRows(TEXT("Client GlobalConfiguration"), ClientGlobalConfiguration_Cache);
		}
	}

	return ClientGlobalConfiguration_Cache[0];
}

