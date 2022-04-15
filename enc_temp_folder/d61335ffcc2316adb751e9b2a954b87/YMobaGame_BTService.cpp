// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGame_BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "../../Character_Instance/YMobaGameCharacter.h"
#include "../AIController/YMobaGameAIController.h"


void UYMobaGame_BTService::InitializeFromAsset(UBehaviorTree& Asset)
{
	if (UBlackboardData* SourceData_BBAsset = GetBlackboardAsset())
	{
		Blackboard_Target.ResolveSelectedKey(*SourceData_BBAsset);
		Blackboard_Distance.ResolveSelectedKey(*SourceData_BBAsset);
		Blackboard_Location.ResolveSelectedKey(*SourceData_BBAsset);
	}
}

void UYMobaGame_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//判断黑板数据是否有效.
	if (Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
	
		//获取 AI Controller.
		if (AYMobaGameAIController* OwnerAIController = Cast<AYMobaGameAIController>(OwnerComp.GetOwner())) {
			//获取黑板组件.
			if (UBlackboardComponent* BlackboardComponent_Ins = OwnerComp.GetBlackboardComponent()) {
				//根据黑板栏目成员获取 Target 栏目的值.
				AYMobaGameCharacter* Enemy = Cast<AYMobaGameCharacter>(BlackboardComponent_Ins->GetValueAsObject(Blackboard_Target.SelectedKeyName));
				
				float Distance_EM = 1001.0f;

				//若获取成功，则说明已通过鼠标点击强制设置了敌人.
				if (Enemy) {					
					//计算发起攻击者与敌人的距离.
					
					Distance_EM = FVector::Dist(Enemy->GetActorLocation(), OwnerAIController->GetPawn()->GetActorLocation());

					//更新黑板中鼠标点击位置与玩家的距离.
					BlackboardComponent_Ins->SetValueAsVector(Blackboard_Location.SelectedKeyName, Enemy->GetActorLocation());
				}
				else {
					//自动寻找敌人.
				}

				//将与当前敌人的距离写回黑板.
				BlackboardComponent_Ins->SetValueAsFloat(Blackboard_Distance.SelectedKeyName, Distance_EM);
			}
		}
		
	}
}