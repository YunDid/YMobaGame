// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGameAttack_BTTaskNode.h"

EBTNodeResult::Type UYMobaGameAttack_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	//判断任务节点所需的黑板条目类型是否为黑板允许的条目类型.
	if (Blackboard_Actor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		//如果是允许的，可操作性的，则通过行为树获取黑板资源.
		if (UBlackboardComponent* Blackboard_Ins = OwnerComp.GetBlackboardComponent()) {
			//获取到黑板资源后，获取 AI Controller 执行 CommonAttack.
			if (AYMobaGameAIController* AIController = Cast<AYMobaGameAIController>(OwnerComp.GetOwner())) {
				//传入黑板资源中的 Enemy Object
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
	
	//？？？？？？？？？？？？？？？？？？？？？？？？？？？注册？？？信息登记？
	if (UBlackboardData* SourceData_BBAsset = GetBlackboardAsset())
	{
		Blackboard_Actor.ResolveSelectedKey(*SourceData_BBAsset);
	}
}