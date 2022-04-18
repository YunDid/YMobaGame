// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameMoveTo_BTTaskNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../../Character_Instance/YMobaGameCharacter.h"
#include "../AIController/YMobaGameAIController.h"
#include "../../../Common/MethodUnit.h"

EBTNodeResult::Type UYMobaGameMoveTo_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//��ȡ�ڰ��� SelfActor ����.
	if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		//��ȡ�ڰ����.
		if (UBlackboardComponent* BlackboardComponent_Ins = OwnerComp.GetBlackboardComponent()) {
			//��ȡ SelfActor ����.
			if (AYMobaGameCharacter* SelfActor = Cast<AYMobaGameCharacter>(BlackboardComponent_Ins->GetValueAsObject("SelfActor"))) {			
				//��ȡ SelfActor �� CharacterID.
				int64 CharacterID_SelfActor = SelfActor->GetCharacterID();
				//���� SelfActor �������ñ����������ù����뾶.
				if (const FCharacterTable* FCharacterTable_SelfActor = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID_SelfActor)) {
					AcceptableRadius = FCharacterTable_SelfActor->CharacterID;
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UYMobaGameMoveTo_BTTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//�ɺڰ���Դ��ʼ���ڰ���Ŀ��Ա.
	if (UBlackboardData* SourceData_BBAsset = GetBlackboardAsset())
	{
		Blackboard_Actor.ResolveSelectedKey(*SourceData_BBAsset);
	}
}
