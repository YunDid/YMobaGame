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

	//�չ��߼�.
	void CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	//���ܹ���.
	void SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy);

	//��ȡ��Ӧ���ܶ�Ӧ�Ķ���.
	UAnimMontage* GetSkillAttackAnimation(KeyCode_Type KeyCode);

	//�㲥���Ŷ���.
	UFUNCTION(NetMulticast,unreliable)
	void MutiCastPlayerAnimMontage(UAnimMontage* AnimMontage_Ins, float PlayRate = 1.0f, FName StartSectionName = NAME_None);

	//����ǰ����µĽ�ɫ���ԵǼǵ� GameState �е���Ӧ������.
	void RegisterPlayerAttributes(const int64& InPlayerID, int32 CharacterID);

	//��ȡ��ɫ��Ӧ����.
	FCharacterAttribute* GetCurrentCharacterAttribute();

	//��ȡ���ID.
	int64 GetPlayerID();

	//�жϽ�ɫ�Ƿ�����.
	bool IsDie();

protected:
	//�Ƿ��ڹ���״̬.
	bool bAttacking;
	//�չ������׶�.
	uint8 Attack_Count;
	//��ɫID�����ڶ�ȡ��Ӧ�Ķ���.
	int64 PlayerID;

};

