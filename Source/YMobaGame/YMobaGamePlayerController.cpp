// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGamePlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MobaPawn.h"
#include "Engine/World.h"
#include "Tool/ScreenMoveUnits.h"

AYMobaGamePlayerController::AYMobaGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AYMobaGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//仅仅在客户端更新
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		// keep updating the destination every tick while desired
		if (bMoveToMouseCursor)
		{
			MoveToMouseCursor();
		}
		//屏幕跟随鼠标移动
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
	/*if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AMobaPawn* MyPawn = Cast<AMobaPawn>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}*/
	if (AMobaPawn* MyPawn = Cast<AMobaPawn>(GetPawn()))
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			MyPawn->CharactorMoveToOnServer(Hit.ImpactPoint);
		}
	}
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
