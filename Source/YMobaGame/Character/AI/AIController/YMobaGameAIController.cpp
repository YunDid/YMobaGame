// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAIController.h"

AYMobaGameAIController::AYMobaGameAIController()
{
	//找寻行为树资源.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> UUBehaviorTree_Ins(TEXT("/Game/Tables/CharacterTable"));
	BTAsset = UUBehaviorTree_Ins.Object;
}

void AYMobaGameAIController::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	EnemyCharacter = Enemy;
	//获取 AI Controller 控制的角色，调用角色底层对 CommonAttack 的实现接口，攻击 Enemy.
	AYMobaGameCharacter* CurrentCharacter = Cast<AYMobaGameCharacter>(GetPawn());
	if (CurrentCharacter) {
		CurrentCharacter->CommonAttack(EnemyCharacter);
	}
}

void AYMobaGameAIController::BeginPlay()
{
	Super::BeginPlay();
	//服务端启动 AI 行为树框架.
	if (GetLocalRole() == ROLE_Authority) {
		RunBehaviorTree(BTAsset);
	}
}