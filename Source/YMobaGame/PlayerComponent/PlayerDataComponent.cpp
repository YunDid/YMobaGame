// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDataComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerDataComponent::UPlayerDataComponent()
{
	PlayerName = "SDA12A@#!!!";
	Team = ETeamType::Team_Red;
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPlayerDataComponent::BeginPlay()
{
	Super::BeginPlay();

	//先从文件读取指定玩家属性数据.

	TArray<FString> InfoTest;
	FFileHelper::LoadFileToStringArray(InfoTest, *(FPaths::ProjectDir() / TEXT("PlayerInfo.txt")));
	if (InfoTest.Num() == 2)
	{
		PlayerName = *InfoTest[0];
		Team = (ETeamType)FCString::Atoi(*InfoTest[0]);
	}
}


// Called every frame
void UPlayerDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerDataComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//仅在初始化时同步
	DOREPLIFETIME_CONDITION(UPlayerDataComponent, PlayerName, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UPlayerDataComponent, Team, COND_InitialOnly);
}
