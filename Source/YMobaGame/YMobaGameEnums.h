// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YMobaGameEnums.generated.h"

enum class KeyCode_Type : uint8
{
	KeyCode_Q,
	KeyCode_W,
	KeyCode_E,
	KeyCode_R
};

UENUM()
enum class ETeamType : uint8
{
	Team_Red,
	Team_Blue,
	Team_Neutrality
};