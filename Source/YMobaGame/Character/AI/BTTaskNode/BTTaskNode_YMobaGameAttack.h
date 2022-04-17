// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AIController/YMobaGameAIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../../Character_Instance/YMobaGameCharacter.h"
#include "BTTaskNode_YMobaGameAttack.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API UBTTaskNode_YMobaGameAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	/** starts this task, should return Succeeded, Failed or InProgress
	 *  (use FinishLatentTask() when returning InProgress)
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}

public:
	//当前任务节点下的需要的黑板条目，即某个键值对.
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector Blackboard_Actor;
	
};
