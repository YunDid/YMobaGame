// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAIController.h"

AYMobaGameAIController::AYMobaGameAIController()
{
	//��Ѱ��Ϊ����Դ.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> UUBehaviorTree_Ins(TEXT("/Game/TopDownCPP/AI/YMobaAI_BT"));
	BTAsset = UUBehaviorTree_Ins.Object;
}

void AYMobaGameAIController::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	EnemyCharacter = Enemy;
	//��ȡ AI Controller ���ƵĽ�ɫ�����ý�ɫ�ײ�� CommonAttack ��ʵ�ֽӿڣ����� Enemy.
	AYMobaGameCharacter* CurrentCharacter = Cast<AYMobaGameCharacter>(GetPawn());
	if (CurrentCharacter) {
		CurrentCharacter->CommonAttack(EnemyCharacter);
	}
}

void AYMobaGameAIController::SimpleMoverTo(const FVector& DirectionLocation) 
{
	//��պڰ��е� Targert,ʹ��Ϊ�������ж�.
	SetAttackTarget(nullptr);
	//�� AI_BT ���ƵĽ�ɫ�߼���ֻ��Ҫ��ֵ��������Ҫ�ĺڰ���Ŀ��ֵ����.
	GetBlackboardComponent()->SetValueAsVector("Location", DirectionLocation);
}

void AYMobaGameAIController::SetAttackTarget(AYMobaGameCharacter* Enemy) {
	GetBlackboardComponent()->SetValueAsObject("Target", Enemy);

	EnemyCharacter = Enemy;
}

void AYMobaGameAIController::BeginPlay()
{
	Super::BeginPlay();
	//��������� AI ��Ϊ�����.
	if (GetLocalRole() == ROLE_Authority) {
		RunBehaviorTree(BTAsset);
	}
}