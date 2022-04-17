// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_YMobaGameService.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API UBTService_YMobaGameService : public UBTService
{
	GENERATED_BODY()
	
public:

	/** update next tick interval
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/** Notify called after GameplayTask finishes initialization (not active yet) */
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) {}

	/** Notify called after GameplayTask changes state from Active (finishing or pausing) */
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) {}

public:
	//ºÚ°å Target À¸Ä¿.
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Blackboard_Target;
	//ºÚ°åÖÐ Distance À¸Ä¿.
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Blackboard_Distance;
	//ºÚ°åÖÐ Location À¸Ä¿.
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Blackboard_Location;
};
