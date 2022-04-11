// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAIController.h"

AYMobaGameAIController::AYMobaGameAIController()
{
	//��Ѱ��Ϊ����Դ.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> UUBehaviorTree_Ins(TEXT("/Game/Tables/CharacterTable"));
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

void AYMobaGameAIController::BeginPlay()
{
	Super::BeginPlay();
	//��������� AI ��Ϊ�����.
	if (GetLocalRole() == ROLE_Authority) {
		RunBehaviorTree(BTAsset);
	}
}