// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InformationBar.h"

void UUI_InformationBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_InformationBar::SetTitle(const FString& Msg)
{
	Title->SetText(FText::FromString(Msg));
}
void UUI_InformationBar::SetHealth(float InPercentage)
{
	Health->SetPercent(InPercentage);
}
void UUI_InformationBar::SetMana(float InPercentage)
{
	Mana->SetPercent(InPercentage);
}
