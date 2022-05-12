#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_MiniMap.generated.h"

class UCanvasPanel;
class ASceneCapture2D;
class UImage;
class UMaterialInterface;
class UCanvasPanelSlot;
/**
 *
 */
UCLASS()
class UUI_MiniMap : public UUI_Base
{
	GENERATED_BODY()

	//С��ͼ����.
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MiniMapPanel;

	//��ͼ��Ƭ.
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapImage;

	//2D��׽.
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<ASceneCapture2D> CaptureClass;

	//����.
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TScriptInterface<UMaterialInterface> MiniMapMaterial;

public:
	//��ʼ��.
	virtual void NativeConstruct() override;
	//��֡.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	//С��ͼ��С.
	FVector2D MapSize;
};
