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

	class UGame_UserWidget * GetHUDWidget();

	void UpdateHealthText(FText NewHealthText);

	void UpdateGoldText(int32 NewGoldCount);

	UFUNCTION()
	void GameOverMode();

private:

	UPROPERTY(EditAnywhere, Category = "Stats")
	TSubclassOf<class UGame_UserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UGame_UserWidget * CurrentWidget;

	class ATower_GameMode* GM;

	UFUNCTION()
	void QuitGameAction();
};
