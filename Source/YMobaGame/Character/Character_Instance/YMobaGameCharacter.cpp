// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../../Common/MethodUnit.h"
#include "Engine/World.h"

AYMobaGameCharacter::AYMobaGameCharacter():
	bAttacking(false), Attack_Count(0), PlayerID(INDEX_NONE)
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

	//�����ã�PlayerIDĿǰ��֪���Ķ���ʼ��.
	PlayerID = FMath::RandRange(1000000, 8888888);

	//���� Widget ���.
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AYMobaGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	//��֡�������λ����Ϣ������ͬ��.
	if (GetLocalRole() == ROLE_Authority)
	{
		if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld()))
		{
			GameState_Ins->UpdateCharacterAILocation(GetPlayerID(), GetActorLocation());
		}
	}

}

void AYMobaGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//�ͻ���ģ��
	if (GetLocalRole() == ROLE_Authority) {
		SpawnDefaultController();
	}
}

void AYMobaGameCharacter::RegisterPlayerAttributes(const int64& InPlayerID, int32 CharacterID)
{
	PlayerID = InPlayerID;

	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld())) {

		GameState_Ins->AddPlayerAttribute(PlayerID, CharacterID);
		GameState_Ins->AddPlayerLocation(PlayerID, GetActorLocation());
		//�����ӳ�ʱ�䲢��ɽ�ɫ�����������õĹ㲥.
		GetWorld()->GetTimerManager().SetTimer(InitTimeHandle, this, &AYMobaGameCharacter::InitCharacteritSelf, 0.1f);		
	}
}

void AYMobaGameCharacter::InitCharacteritSelf()
{
	//�ӳٳ�ʼ��
	/*
		ԭ��:
		�ͻ��˵Ľ�ɫ�Ƿ����ͬ�������ģ��ͻ��˱���������ɫ��ֻ���Ƹ���.
		�ͻ��˵Ľ�ɫ��ʼ�����������ڷ����ͬ��֮��ʼ.
	*/

	//
	if (InitTimeHandle.IsValid()) {	
		GetWorld()->GetTimerManager().ClearTimer(InitTimeHandle);
	}
	//��ȡ��ɫ���ñ��е� UI_Info ��㲥.
	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld())) 
	{
		if (FCharacterAttribute* CharacterAttribute_Ins = GameState_Ins->GetCharacterAttributeByID(PlayerID)) {
			MutiCastWidgetInfo(CharacterAttribute_Ins->GetHealthPercentage(), CharacterAttribute_Ins->GetManaPercentage());
		}
	}

}



int64 AYMobaGameCharacter::GetPlayerID() 
{
	return PlayerID;
}

void AYMobaGameCharacter::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	if (Enemy.IsValid()) {

		//��ȡPlayerID ��Ӧ�Ľ�ɫ���õ� ID.
		int32 CharacterID = MethodUnit::GetCurrentCharacterID(GetWorld(), PlayerID);
		if (CharacterID != INDEX_NONE) {
			//��ȡ CharacterID �µ����ñ���.
			if (const FCharacterTable* CharacterConfig = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
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
}

UAnimMontage* AYMobaGameCharacter::GetSkillAttackAnimation(KeyCode_Type KeyCode) 
{
	//��ȡPlayerID ��Ӧ�Ľ�ɫ���õ� ID.
	int32 CharacterID = MethodUnit::GetCurrentCharacterID(GetWorld(), PlayerID);
	if (CharacterID != INDEX_NONE) {
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

void AYMobaGameCharacter::MutiCastWidgetInfo_Implementation(float HeathPercentage, float ManaPercentage)
{
	//���ڿͻ�����ɽ�ɫUI�������.
	if (GetLocalRole() != ROLE_Authority) {
		//��ȡ��ǰ��ɫ�� Widget ���ת��Ϊ�ײ������ͺ� UI Info ��������.
		if (UUI_InformationBar* Widget_Ins = Cast<UUI_InformationBar>(Widget->GetUserWidgetObject())) {
			Widget_Ins->SetHealth(HeathPercentage);
			Widget_Ins->SetMana(ManaPercentage);
		}
	}
}


FCharacterAttribute* AYMobaGameCharacter::GetCurrentCharacterAttribute()
{
	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld()))
	{
		return GameState_Ins->GetCharacterAttributeByID(PlayerID);
	}

	return NULL;
}

bool AYMobaGameCharacter::IsDie()
{
	if (GetCurrentCharacterAttribute()->Health <= 0.f)
	{
		return true;
	}

	return false;
}







