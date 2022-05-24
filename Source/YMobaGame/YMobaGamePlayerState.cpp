// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGamePlayerState.h"

AYMobaGamePlayerState::AYMobaGamePlayerState()
{
	PlayerData = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("Player Data"));
}

const UPlayerDataComponent* AYMobaGamePlayerState::GetPlayerData()
{ 
	return PlayerData;
}

