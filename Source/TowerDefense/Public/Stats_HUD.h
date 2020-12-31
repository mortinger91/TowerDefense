// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Stats_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AStats_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AStats_HUD();
	
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	UUserWidget * GetHUDWidget();

	void UpdateGoldText(int32 NewGoldCount);

private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UUserWidget * CurrentWidget;

};
