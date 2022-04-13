// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../../Character_Instance/YMobaGameCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YMobaGameAIController.generated.h"

/**
 * 
 */
UCLASS()
class YMOBAGAME_API AYMobaGameAIController : public AAIController
{
	GENERATED_BODY()

public:

	AYMobaGameAIController();

	//ÆÕÍ¨¹¥»÷.
	void CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy);
	void SimpleMoverTo(const FVector& DirectionLocation);
	void SetAttackTarget(AYMobaGameCharacter* Enemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TWeakObjectPtr<AYMobaGameCharacter> EnemyCharacter;

public:
	UPROPERTY()
	UBehaviorTree* BTAsset;
};
