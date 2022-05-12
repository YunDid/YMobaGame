#include "RenderingUnit.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "Landscape.h"
#include "Engine/SceneCapture2D.h"
#include "EngineUtils.h"

//防止优化
#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

namespace RenderingUnit
{
	FScreenShot::FScreenShot(
		int32 InWidth,
		int32 InHeight,
		UTexture2D*& InTexture,
		UObject* InOuter,
		int32 InImageQuality /*= 80*/,
		bool bInShowUI /*= false*/,
		bool bAddFilenameSuffix /*= true*/)
		:Texture(InTexture)
		, ScaledWidth(InWidth)
		, ScaledHeight(InHeight)
		, ImageQuality(InImageQuality)
		, Outer(InOuter)
	{
		if (!UGameViewportClient::OnScreenshotCaptured().IsBound())
		{
			Filename = FPaths::ProjectSavedDir() / TEXT("SaveGames") / FGuid::NewGuid().ToString();
			ScreenShotDelegateHandle = UGameViewportClient::OnScreenshotCaptured().AddRaw(
				this,
				&RenderingUnit::FScreenShot::OnScreenshotCapturedInternal);

			FScreenshotRequest::RequestScreenshot(Filename, bInShowUI, bAddFilenameSuffix);
			Filename += TEXT(".jpg");
		}
	}

	void FScreenShot::OnScreenshotCapturedInternal(
		int32 SrcWidth,
		int32 SrcHeight,
		const TArray<FColor>& OrigBitmap)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		check(OrigBitmap.Num() == SrcWidth * SrcHeight);

		// 调整图像大小以强制最大大小。 
		TArray<FColor> ScaledBitmap;
		FImageUtils::ImageResize(SrcWidth, SrcHeight, OrigBitmap, ScaledWidth, ScaledHeight, ScaledBitmap, true);

		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(ScaledBitmap.GetData(), ScaledBitmap.GetAllocatedSize(), ScaledWidth, ScaledHeight, ERGBFormat::BGRA, 8);

		//jpg资源包头
		const TArray64<uint8>& JPEGData = ImageWrapper->GetCompressed(ImageQuality);
		FFileHelper::SaveArrayToFile(JPEGData, *Filename);

		//压缩
		FCreateTexture2DParameters Params;
		Params.bDeferCompression = true;
		Texture = FImageUtils::CreateTexture2D(ScaledWidth, ScaledHeight, ScaledBitmap, Outer, FGuid::NewGuid().ToString(), RF_NoFlags, Params);

		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenShotDelegateHandle);
		ImageWrapper.Reset();

		//结束自己
		delete this;
	}

	ASceneCapture2D* SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FVector2D& MapSize, float Life)
	{
		if (SceneCaptureClass)
		{
			for (TActorIterator<ALandscape> It(World); It; ++It)
			{
				if (ALandscape* BigMap = *It)
				{
					//都是正方形
					FVector BigMapSize = BigMap->GetActorScale3D();
					MapSize = FVector2D(BigMapSize.X * 40, BigMapSize.Y * 40);

					FVector CenterPoint = FVector(MapSize.X / 2);

					if (ASceneCapture2D* NewCarma = World->SpawnActor<ASceneCapture2D>(SceneCaptureClass, CenterPoint, FRotator(-90.f, 0.f, 0.f)))
					{
						if (Life != 0.f)
						{
							NewCarma->SetLifeSpan(Life);
						}

						return NewCarma;
					}

					break;
				}
			}
		}

		return nullptr;
	}

}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
