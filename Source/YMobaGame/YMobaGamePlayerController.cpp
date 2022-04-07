// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGamePlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MobaPawn.h"
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

void AYMobaGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AYMobaGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AYMobaGamePlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AYMobaGamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AYMobaGamePlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AYMobaGamePlayerController::OnResetVR);
}

void AYMobaGamePlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AYMobaGamePlayerController::MoveToMouseCursor()
{

	if (AMobaPawn* MyPawn = Cast<AMobaPawn>(GetPawn()))
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
					VerifyMouseClickOnServer_Implementation(WorldOrigin, WorldDirection);
				}			
			}
			
		}
	}
}

void AYMobaGamePlayerController::VerifyMouseClickOnServer_Implementation(const FVector& WorldOrigin, const FVector& WorldDirection)
{
	if (AMobaPawn* MyPawn = Cast<AMobaPawn>(GetPawn()))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);

		//���� ECC_GameTraceChannel1 �����׷��
		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, CollisionQueryParams)) {
			if (HitResult.bBlockingHit) {
				//��⵽���ƶ�����
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
	if (AMobaPawn* MyPawn = Cast<AMobaPawn>(GetPawn())) {
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
