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

	//MiniMap 最大大小.
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MaxMiniMapSize;
	//MiniMap 当前大小.
	UPROPERTY(EditDefaultsOnly, Category = "MiniMap")
	float MiniMapSize;
	//相机弹簧臂 最大大小.
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxTargetArmLength;
	//相机弹簧臂 当前大小.
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TargetArmLength;

};
