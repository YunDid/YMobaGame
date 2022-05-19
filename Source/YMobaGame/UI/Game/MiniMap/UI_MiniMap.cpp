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
	if (ASceneCapture2D* SceneCapture2D = RenderingUnit::SpawnSceneCapture2D(GetWorld(), CaptureClass, MapSize, 0.3f))
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

	if (AYMobaGameState* InState = GetWorld()->GetGameState<AYMobaGameState>())
	{
		const TArray<FPlayerLocation>& Locations = InState->GetPlayerLocations();

		if (UCanvasPanelSlot* ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
		{
			const float MaxMiniMapSize = GetHUD()->GetClientGlobalConfiguration()->MaxMiniMapSize;
			const float MaxTargetArmLength = GetHUD()->GetClientGlobalConfiguration()->MaxTargetArmLength;

			FVector2D LocalSize2D = MyGeometry.GetDrawSize();

			//地图缩放
			FVector2D ZoomSize;
			{
				float TargetArmLength = 800.f;//Cast<ATowerDefenceGameCamera>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetTargetArmLength();
				ZoomSize = FVector2D((MaxMiniMapSize / MaxTargetArmLength) * TargetArmLength);

				ImagePanelSlot->SetSize(ZoomSize);
			}

			//地图位置
			FVector2D MinImageMapOffset;
			{
				FVector Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				MinImageMapOffset.Y = -(ZoomSize.X - (ZoomSize.X / MapSize.X) * Location.X);
				MinImageMapOffset.X = -(ZoomSize.Y / MapSize.Y) * Location.Y;

				ImagePanelSlot->SetPosition(MinImageMapOffset);
			}

			//场景中的角色
			for (auto& Tmp : Locations)
			{
				if (!IsExistence(Tmp.PlayerID))
				{
					if (UImage* Point = NewObject<UImage>(GetWorld(), UImage::StaticClass()))
					{
						if (UCanvasPanelSlot* PanelSlot = MiniMapPanel->AddChildToCanvas(Point))
						{
							PanelSlot->SetZOrder(1.0f);
							PanelSlot->SetAnchors(0.5f);
							//Point->SetBrushFromTexture(Tmp.Value.Icon.LoadSynchronous());
							PanelSlot->SetSize(FVector2D(32.f));
							PanelSlot->SetAlignment(FVector2D(.5f));
							CharacterIcons.Add(Tmp.PlayerID, PanelSlot);
						}
					}
				}
				else
				{
					if (UCanvasPanelSlot* PanelSlot = CharacterIcons[Tmp.PlayerID].Get())
					{
						FVector2D MinMapPos;
						MinMapPos.Y = ZoomSize.X - (ZoomSize.X / MapSize.X) * Tmp.Location.X + MinImageMapOffset.Y;
						MinMapPos.X = (ZoomSize.Y / MapSize.Y) * Tmp.Location.Y + MinImageMapOffset.X;

						ResetLocation(PanelSlot, MinMapPos, LocalSize2D);
					}
				}

			}

			TArray<int64> RemoveGuid;
			for (auto& Tmp : CharacterIcons)
			{
				bool bContains = false;
				for (auto& L : Locations)
				{
					if (L.PlayerID == Tmp.Key)
					{
						bContains = true;
						break;
					}
				}

				if (!bContains)
				{
					if (UCanvasPanelSlot* PanelSlot = CharacterIcons[Tmp.Key].Get())
					{
						MiniMapPanel->RemoveChild(PanelSlot->Content);
						RemoveGuid.Add(Tmp.Key);
					}
				}
			}

			for (auto& Tmp : RemoveGuid)
			{
				CharacterIcons.Remove(Tmp);
			}
		}

	}

}

bool UUI_MiniMap::IsExistence(const int64 InID)
{
	for (auto& Icon : CharacterIcons)
	{
		if (Icon.Key == InID)
		{
			return true;
		}
	}

	return false;
}

void UUI_MiniMap::ResetLocation(UCanvasPanelSlot* PanelSlot, const FVector2D& MinMapPos, const FVector2D& LocalSize2D)
{
	FVector2D IconSize = PanelSlot->GetSize() / 2;

	auto IsRange = [&](const float& CompareValue, const float& Value)->bool
	{
		return Value > -CompareValue && Value < CompareValue;
	};

	FVector2D NewPos = MinMapPos;
	if (MinMapPos.X <= LocalSize2D.X &&
		MinMapPos.Y <= LocalSize2D.Y &&
		MinMapPos.X >= -LocalSize2D.X &&
		MinMapPos.Y >= -LocalSize2D.Y)
	{

	}
	else if (MinMapPos.Y < -LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//上
	{
		NewPos = FVector2D(MinMapPos.X, -LocalSize2D.Y + IconSize.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//左
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.Y, MinMapPos.Y);
	}
	else if (MinMapPos.Y > LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//下
	{
		NewPos = FVector2D(MinMapPos.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//右
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, MinMapPos.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//左上
	{
		NewPos = -LocalSize2D + IconSize;
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//左下
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//右下
	{
		NewPos = LocalSize2D - IconSize;
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//右上
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, -LocalSize2D.Y + IconSize.Y);
	}

	PanelSlot->SetPosition(NewPos);
}
