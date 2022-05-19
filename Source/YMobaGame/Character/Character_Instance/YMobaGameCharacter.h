// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../YMobaGameEnums.h"
#include "../../Table/CharacterAttribute.h"
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

	//普攻逻辑.
	void CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	//技能攻击.
	void SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	//获取相应技能对应的动画.
	UAnimMontage* GetSkillAttackAnimation(KeyCode_Type KeyCode);

	//广播播放动画.
	UFUNCTION(NetMulticast,unreliable)
	void MutiCastPlayerAnimMontage(UAnimMontage* AnimMontage_Ins, float PlayRate = 1.0f, FName StartSectionName = NAME_None);

	//将当前玩家下的角色属性登记到 GameState 中的相应容器中.
	void RegisterPlayerAttributes(const int64& InPlayerID, int32 CharacterID);

	//获取角色相应属性.
	FCharacterAttribute* GetCurrentCharacterAttribute();

	//获取玩家ID.
	int64 GetPlayerID();

	//判断角色是否死亡.
	bool IsDie();

protected:
	//是否处于攻击状态.
	bool bAttacking;
	//普攻所处阶段.
	uint8 Attack_Count;
	//角色ID，便于读取对应的动画.
	int64 PlayerID;

};

