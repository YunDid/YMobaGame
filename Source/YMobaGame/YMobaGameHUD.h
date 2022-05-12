// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/Config/GlobalConfiguration_Client.h"
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

	//获取客户端全局配置表.
	const FGlobalConfiguration_Client* GetClientGlobalConfiguration();
private:
	//列表项类型为 GlobalConfiguration_Client 的 UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_ClientGlobalConfiguration;

	//缓存用于存储 UDataTable_GlobalConfiguration_Client 所有配置表表项.
	//但是由于只有一项，因此只需要 TArray[0] 对应的表项.
	TArray<FGlobalConfiguration_Client*> ClientGlobalConfiguration_Cache;

	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
};
