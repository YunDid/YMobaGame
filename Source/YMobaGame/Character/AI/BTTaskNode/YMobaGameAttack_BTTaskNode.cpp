// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAttack_BTTaskNode.h"

EBTNodeResult::Type UYMobaGameAttack_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	//�ж�����ڵ�����ĺڰ���Ŀ�����Ƿ�Ϊ�ڰ��������Ŀ����.
	if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		//���������ģ��ɲ����Եģ���ͨ����Ϊ����ȡ�ڰ���Դ.
		if (UBlackboardComponent* Blackboard_Ins = OwnerComp.GetBlackboardComponent()) {
			//��ȡ���ڰ���Դ�󣬻�ȡ AI Controller ִ�� CommonAttack.
			if (AYMobaGameAIController* AIController = Cast<AYMobaGameAIController>(OwnerComp.GetOwner())) {
				//����ڰ���Դ�е� Enemy Object
				if (AYMobaGameCharacter* Enemy = Cast<AYMobaGameCharacter>(Blackboard_Ins->GetValueAsObject(Blackboard_Actor.SelectedKeyName))) {
					
					AIController->CommonAttack(Enemy);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UYMobaGameAttack_BTTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	//������������������������������������������������������ע�᣿������Ϣ�Ǽǣ�
	if (UBlackboardData* SourceData_BBAsset = GetBlackboardAsset())
	{
		Blackboard_Actor.ResolveSelectedKey(*SourceData_BBAsset);
	}
}