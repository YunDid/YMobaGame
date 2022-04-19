// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../../Common/MethodUnit.h"
#include "Engine/World.h"

AYMobaGameCharacter::AYMobaGameCharacter():
	bAttacking(false), Attack_Count(0), CharacterID(INDEX_NONE)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AYMobaGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}

void AYMobaGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//�ͻ���ģ��
	if (GetLocalRole() == ROLE_Authority) {
		SpawnDefaultController();
	}
}

void AYMobaGameCharacter::InitCharacterID(const int64& ID) {
	CharacterID = ID;
}

int64 AYMobaGameCharacter::GetCharacterID() {
	return CharacterID;
}

void AYMobaGameCharacter::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	if (Enemy.IsValid()) {
		//��ȡ CharacterID �µ����ñ���.
		if (const FCharacterTable* CharacterConfig =  MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
			//�жϵ�ǰ�չ����ڵڼ��׶�.
			if (Attack_Count < CharacterConfig->CommonAttack_Animation.Num()) {

				//��ȡ CharacterID �µľ����չ�����.
				if (UAnimMontage* Attack_AniMontage = CharacterConfig->CommonAttack_Animation[Attack_Count]) {
					//���Ź�������.
					MutiCastPlayerAnimMontage(Attack_AniMontage);

					//�չ�״̬��������.
					if (Attack_Count == CharacterConfig->CommonAttack_Animation.Num() - 1) {
						//���Ѵ��ڵ����ڶ��׶Σ�������.
						Attack_Count = 0;
					}
					else {
						Attack_Count++;
					}
				}
			}
		}
	}
}

UAnimMontage* AYMobaGameCharacter::GetSkillAttackAnimation(KeyCode_Type KeyCode) 
{
	//��ȡ CharacterID �µ����ñ���.
	if (const FCharacterTable* CharacterConfig = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
		
		//���ݼ������ͣ�������Ӧ�ļ��ܶ���.
		switch (KeyCode) {
		case KeyCode_Type::KeyCode_Q: {
			return CharacterConfig->SkillAttack_Q_Animation;
		}; break;
		case KeyCode_Type::KeyCode_W: {
			return CharacterConfig->SkillAttack_W_Animation;
		}; break;
		case KeyCode_Type::KeyCode_E: {
			return CharacterConfig->SkillAttack_E_Animation;
		}; break;
		case KeyCode_Type::KeyCode_R: {
			return CharacterConfig->SkillAttack_R_Animation;
		}; break;
		}
	}

	return nullptr;
}


void AYMobaGameCharacter::SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy) 
{	
	//��ȡ KeyCode �µľ��弼�ܶ���.
	if (UAnimMontage* Attack_AniMontage = GetSkillAttackAnimation(KeyCode)) {
		//���Ź�������.
		MutiCastPlayerAnimMontage(Attack_AniMontage);
	}
}


void AYMobaGameCharacter::MutiCastPlayerAnimMontage_Implementation(UAnimMontage* AnimMontage_Ins, float PlayRate = 1.0f, FName StartSectionName = NAME_None) 
{
	if (AnimMontage_Ins) {
		PlayAnimMontage(AnimMontage_Ins, PlayRate, StartSectionName);
	}
}



