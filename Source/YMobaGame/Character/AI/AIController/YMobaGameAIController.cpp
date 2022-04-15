// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAIController.h"

AYMobaGameAIController::AYMobaGameAIController()
{
	//找寻行为树资源.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> UUBehaviorTree_Ins(TEXT("/Game/TopDownCPP/AI/YMobaAI_BT"));
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

void AYMobaGameAIController::SimpleMoverTo(const FVector& DirectionLocation) 
{
	//清空黑板中的 Targert,使行为树更新判断.
	SetAttackTarget(nullptr);
	//由 AI_BT 控制的角色逻辑，只需要赋值任务结点需要的黑板条目键值即可.
	GetBlackboardComponent()->SetValueAsVector("Location", DirectionLocation);
}

void AYMobaGameAIController::SetAttackTarget(AYMobaGameCharacter* Enemy) {
	GetBlackboardComponent()->SetValueAsObject("Target", Enemy);

	EnemyCharacter = Enemy;
}

void AYMobaGameAIController::BeginPlay()
{
	Super::BeginPlay();
	//服务端启动 AI 行为树框架.
	if (GetLocalRole() == ROLE_Authority) {
		RunBehaviorTree(BTAsset);
	}
}