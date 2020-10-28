// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class TECENTLEARNPROJECT_API ATPSHUD : public AHUD
{
	GENERATED_BODY()
protected:
    // 将在屏幕中央绘制此项。
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrosshairTexture;
public:
    // HUD的主绘制调用。
    virtual void DrawHUD() override;
};
