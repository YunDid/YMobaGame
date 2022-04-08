// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MobaPawn.h"
#include "GameFramework/PlayerController.h"
#include "YMobaGamePlayerController.generated.h"

UCLASS()
class AYMobaGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AYMobaGamePlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UFUNCTION(server, reliable, WithValidation)
	void VerifyMouseClickOnServer(const FVector& WorldOrigin, const FVector& WorldDirection);

};


