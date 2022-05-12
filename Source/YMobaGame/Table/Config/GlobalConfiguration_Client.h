// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GlobalConfiguration_Client.generated.h"

USTRUCT(BluePrintType)
struct FGlobalConfiguration_Client : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FGlobalConfiguration_Client();

	//MiniMap ����С.
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MaxMiniMapSize;
	//MiniMap ��ǰ��С.
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MiniMapSize;
	//������ɱ� ����С.
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxTargetArmLength;
	//������ɱ� ��ǰ��С.
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TargetArmLength;

};
