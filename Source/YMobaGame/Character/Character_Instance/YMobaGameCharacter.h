// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../YMobaGameEnums.h"
#include "YMobaGameCharacter.generated.h"

UCLASS(Blueprintable)
class AYMobaGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AYMobaGameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	UAnimMontage* GetSkillAttackAnimation(KeyCode_Type KeyCode);

	void SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	UFUNCTION(NetMulticast,unreliable)
	void MutiCastPlayerAnimMontage(UAnimMontage* AnimMontage_Ins, float PlayRate = 1.0f, FName StartSectionName = NAME_None);

	void InitCharacterID(const int64& ID);

	int64 GetCharacterID();

protected:
	//是否处于攻击状态.
	bool bAttacking;
	//普攻所处阶段.
	uint8 Attack_Count;
	//角色ID，便于读取对应的动画.
	int64 CharacterID;

};

