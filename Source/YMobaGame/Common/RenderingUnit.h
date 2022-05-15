// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class ASceneCapture2D;

namespace RenderingUnit
{
	struct FScreenShot
	{
	public:
		FScreenShot(
			int32 InWidth,
			int32 InHeight,
			UTexture2D*& InTexture,
			UObject* InOuter,
			int32 InImageQuality = 80,
			bool bInShowUI = false,
			bool bAddFilenameSuffix = true);

		FString& GetFilename() { return Filename; }

	protected:
		void OnScreenshotCapturedInternal(int32 SrcWidth, int32 SrcHeight, const TArray<FColor>& OrigBitmap);

	private:
		int32 ScaledWidth;
		int32 ScaledHeight;
		UTexture2D*& Texture;
		UObject* Outer;
		int32 ImageQuality;
		FDelegateHandle ScreenShotDelegateHandle;
		FString Filename;
	};	

	ASceneCapture2D* SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FVector2D& MapSize, float Life = 0.f);
}
