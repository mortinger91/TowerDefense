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
	
	void UpdateHealthText(FText NewHealthText);

	void UpdateGoldText(int32 NewGoldCount);

	UFUNCTION()
	void PlayGameOverAnimation();

	void ShowTowerTooltip();

	void HideTowerTooltip();

private:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Stats")
	TSubclassOf<class UGame_UserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UGame_UserWidget * GameUIWidget;

	class ATower_GameMode* GM;

	UFUNCTION()
	void QuitGameAction();

	UFUNCTION()
	void LevelUpAction();

	UFUNCTION()
	void SellAction();

	UFUNCTION()
	void SpawnTowerCannonAction();
};
