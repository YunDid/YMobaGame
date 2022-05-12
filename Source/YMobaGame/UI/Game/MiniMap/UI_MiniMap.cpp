// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MiniMap.h"
#include "../../../YMobaGameState.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Engine/SceneCapture2D.h"
#include "../../../Common/RenderingUnit.h"

void UUI_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	//设置 MiniMapImage 的画笔.
	if (ASceneCapture2D* SceneCapture2D = RenderingUnit::SpawnSceneCapture2D(GetWorld(), CaptureClass, MapSize, 0.03f))
	{
		MiniMapImage->SetBrushFromMaterial(Cast<UMaterialInterface>(MiniMapMaterial.GetObject()));
	}

	//设置 MiniMap 的最大大小.
	if (MiniMapImage)
	{
		if (UCanvasPanelSlot* ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
		{
			ImagePanelSlot->SetSize(FVector2D(GetHUD()->GetClientGlobalConfiguration()->MaxMiniMapSize));
		}
	}

}

void UUI_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
