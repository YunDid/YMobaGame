// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_YMobaGameMoveTo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../../Character_Instance/YMobaGameCharacter.h"
#include "../AIController/YMobaGameAIController.h"
#include "../../../Common/MethodUnit.h"

EBTNodeResult::Type UBTTaskNode_YMobaGameMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//获取黑板中 SelfActor 类型.
	if (Blackboard_SelfActor.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		//获取黑板组件.
		if (UBlackboardComponent* BlackboardComponent_Ins = OwnerComp.GetBlackboardComponent()) {
			//获取 SelfActor 对象.
			if (AYMobaGameCharacter* SelfActor = Cast<AYMobaGameCharacter>(BlackboardComponent_Ins->GetValueAsObject("SelfActor"))) {			
				//获取 SelfActor 的 CharacterID.
				int64 CharacterID_SelfActor = SelfActor->GetCharacterID();
				//根据 SelfActor 所属配置表类型来设置攻击半径.
				if (const FCharacterTable* FCharacterTable_SelfActor = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID_SelfActor)) {
					AcceptableRadius = FCharacterTable_SelfActor->AttackRadius;
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTaskNode_YMobaGameMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	//由黑板资源初始化黑板栏目成员.
	if (UBlackboardData* SourceData_BBAsset = GetBlackboardAsset())
	{
		Blackboard_SelfActor.ResolveSelectedKey(*SourceData_BBAsset);
	}
}
