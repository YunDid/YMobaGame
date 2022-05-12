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

	//��ȡ�ͻ���ȫ�����ñ�.
	const FGlobalConfiguration_Client* GetClientGlobalConfiguration();
private:
	//�б�������Ϊ GlobalConfiguration_Client �� UDataTable.
	UPROPERTY()
	UDataTable* UDataTable_ClientGlobalConfiguration;

	//�������ڴ洢 UDataTable_GlobalConfiguration_Client �������ñ����.
	//��������ֻ��һ����ֻ��Ҫ TArray[0] ��Ӧ�ı���.
	TArray<FGlobalConfiguration_Client*> ClientGlobalConfiguration_Cache;

	TSubclassOf<UUI_MainScreen> MainScreenClass;
	UUI_MainScreen* MainScreen;
};
