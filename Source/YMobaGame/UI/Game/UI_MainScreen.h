// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_MainScreen.generated.h"

/**
 *
 */
UCLASS()
class UUI_MainScreen : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UUI_MiniMap* MiniMapSystem;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
