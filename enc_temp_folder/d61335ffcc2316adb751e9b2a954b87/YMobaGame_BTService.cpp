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

	//�жϺڰ������Ƿ���Ч.
	if (Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
	
		//��ȡ AI Controller.
		if (AYMobaGameAIController* OwnerAIController = Cast<AYMobaGameAIController>(OwnerComp.GetOwner())) {
			//��ȡ�ڰ����.
			if (UBlackboardComponent* BlackboardComponent_Ins = OwnerComp.GetBlackboardComponent()) {
				//���ݺڰ���Ŀ��Ա��ȡ Target ��Ŀ��ֵ.
				AYMobaGameCharacter* Enemy = Cast<AYMobaGameCharacter>(BlackboardComponent_Ins->GetValueAsObject(Blackboard_Target.SelectedKeyName));
				
				float Distance_EM = 1001.0f;

				//����ȡ�ɹ�����˵����ͨ�������ǿ�������˵���.
				if (Enemy) {					
					//���㷢�𹥻�������˵ľ���.
					
					Distance_EM = FVector::Dist(Enemy->GetActorLocation(), OwnerAIController->GetPawn()->GetActorLocation());

					//���ºڰ��������λ������ҵľ���.
					BlackboardComponent_Ins->SetValueAsVector(Blackboard_Location.SelectedKeyName, Enemy->GetActorLocation());
				}
				else {
					//�Զ�Ѱ�ҵ���.
				}

				//���뵱ǰ���˵ľ���д�غڰ�.
				BlackboardComponent_Ins->SetValueAsFloat(Blackboard_Distance.SelectedKeyName, Distance_EM);
			}
		}
		
	}
}