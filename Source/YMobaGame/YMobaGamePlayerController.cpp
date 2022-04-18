// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGamePlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine/World.h"
#include "Tool/ScreenMoveUnits.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"

AYMobaGamePlayerController::AYMobaGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AYMobaGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//�����ڿͻ��˸���
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		// keep updating the destination every tick while desired
		if (bMoveToMouseCursor)
		{
			MoveToMouseCursor();
		}
		//��Ļ��������ƶ�,�ɵ����ٶ�
		FScreenMoveUnits().ListenScreenMove(this, 30.0f);
	}
}

void AYMobaGamePlayerController::SkillAttack_Q_Pressed()
{
	if (AYMobaGamePawn* MyPawn = Cast<AYMobaGamePawn>(GetPawn())) {
		
	}
}
void AYMobaGamePlayerController::SkillAttack_Q_Released()
{}
void AYMobaGamePlayerController::SkillAttack_W_Pressed()
{}
void AYMobaGamePlayerController::SkillAttack_W_Released()
{}
void AYMobaGamePlayerController::SkillAttack_E_Pressed()
{}
void AYMobaGamePlayerController::SkillAttack_E_Released()
{}
void AYMobaGamePlayerController::SkillAttack_R_Pressed()
{}
void AYMobaGamePlayerController::SkillAttack_R_Released()
{}

void AYMobaGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AYMobaGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AYMobaGamePlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AYMobaGamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AYMobaGamePlayerController::MoveToTouchLocation);

	//Skills Attack.
	InputComponent->BindAction("KeyCode_Q", IE_Pressed, this, &AYMobaGamePlayerController::SkillAttack_Q_Pressed);
	InputComponent->BindAction("KeyCode_Q", IE_Released, this, &AYMobaGamePlayerController::SkillAttack_Q_Released);

	InputComponent->BindAction("KeyCode_W", IE_Pressed, this, &AYMobaGamePlayerController::SkillAttack_W_Pressed);
	InputComponent->BindAction("KeyCode_W", IE_Released, this, &AYMobaGamePlayerController::SkillAttack_W_Released);

	InputComponent->BindAction("KeyCode_E", IE_Pressed, this, &AYMobaGamePlayerController::SkillAttack_E_Pressed);
	InputComponent->BindAction("KeyCode_E", IE_Released, this, &AYMobaGamePlayerController::SkillAttack_E_Released);

	InputComponent->BindAction("KeyCode_R", IE_Pressed, this, &AYMobaGamePlayerController::SkillAttack_R_Pressed);
	InputComponent->BindAction("KeyCode_R", IE_Released, this, &AYMobaGamePlayerController::SkillAttack_R_Released);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AYMobaGamePlayerController::OnResetVR);
}

void AYMobaGamePlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AYMobaGamePlayerController::MoveToMouseCursor()
{

	if (AYMobaGamePawn* MyPawn = Cast<AYMobaGamePawn>(GetPawn()))
	{
		//��ȡ LocalPlayer.
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

		//ͨ�� LocalPlayer ��ȡ��ǰ ViewportClient �µ� MousePosition.
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FVector2D MousePosition;

			if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
			{				
				// Early out if we clicked on a HUD hitbox
				if (GetHUD() != NULL && GetHUD()->GetHitBoxAtCoordinates(MousePosition, true))
				{
					return;
				}

				//����Ļ��ά����ת��Ϊ�������괫�ݸ�������.
				FVector WorldOrigin;
				FVector WorldDirection;

				if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection) == true)
				{
					VerifyMouseClickOnServer(WorldOrigin, WorldDirection);
				}			
			}
			
		}
	}
}

void AYMobaGamePlayerController::VerifyMouseClickOnServer_Implementation(const FVector& WorldOrigin, const FVector& WorldDirection)
{
	if (AYMobaGamePawn* MyPawn = Cast<AYMobaGamePawn>(GetPawn()))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);

		//���� ECC_GameTraceChannel1 �����׷��
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, CollisionQueryParams)) {
			if (HitResult.bBlockingHit) {
				//��⵽���ƶ�����
				MyPawn->MoveToEnemyAndAttackOnServer(HitResult.ImpactPoint, Cast<APawn>(HitResult.Actor));
				return;
			}
		}

		//���� ECC_Visibility �����׷��
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_Visibility, CollisionQueryParams)) {
			
			if (HitResult.bBlockingHit) {
				//��ɫ�ƶ�
				MyPawn->CharactorMoveToOnServer(HitResult.ImpactPoint);
			}	
		}
	}
}

bool AYMobaGamePlayerController::VerifyMouseClickOnServer_Validate(const FVector& WorldOrigin, const FVector& WorldDirection)
{
	return true;
}

void AYMobaGamePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (AYMobaGamePawn* MyPawn = Cast<AYMobaGamePawn>(GetPawn())) {
		FVector2D ScreenSpaceLocation(Location);

		// Trace to see what is under the touch location
		FHitResult HitResult;
		GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
		if (HitResult.bBlockingHit)
		{
			// We hit something, move there
			MyPawn->CharactorMoveToOnServer(HitResult.ImpactPoint);
		}
	}
}

void AYMobaGamePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AYMobaGamePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
