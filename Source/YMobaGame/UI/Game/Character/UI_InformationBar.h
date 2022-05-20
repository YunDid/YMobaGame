// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../UI/Core/UI_Base.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UI_InformationBar.generated.h"

/**
 * 
 */

UCLASS()
class YMOBAGAME_API UUI_InformationBar : public UUI_Base
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetTitle(const FString& Msg);
	void SetHealth(float InPercentage);
	void SetMana(float InPercentage);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Health;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Mana;
};
