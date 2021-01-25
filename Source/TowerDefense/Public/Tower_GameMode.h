// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Tower_GameMode.generated.h"

//enum to store the current state of gameplay
UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS()
class TOWERDEFENSE_API ATower_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATower_GameMode();

	virtual void BeginPlay() override;

	virtual void Tick (float DeltaTime) override;

	// health bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PreviousHealth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UCurveFloat* HealthCurve;

	float CurveFloatValue;
	float TimelineValue;
	class UTimelineComponent* MyTimeline;

	UFUNCTION()
	void SetHealth();

	void Remove1Health();

	// max gold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 MaxGold;

	void UpdateGold(float gold);

	bool GoldAvailable(int32 GoldToCheck);

	// Handle any function calls that rely upon changing the playing state of our game
	void ChangeGamePlayState(EGamePlayState NewState);

	class ATower * selectedTower;

	void LevelUpSelectedTower();

	// player hud, update gold count when enemy dies
	class AStats_HUD * HudWidgetPlayer;

	bool dragMode;

	void SpawnTower(FString towerType);

	class ATower * spawnedTower;

	void FinalizeTowerSpawn(); 

private:
	//TArray<FVector> EndPointPosition;

	class ATower_GameState* GS;
	
	// Keeps track of the current playing state
	EGamePlayState CurrentState;

	UFUNCTION()
	void GoToMainMenu();

	bool InitiateGameOver;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	//TSubclassOf<class ATower> CannonTower;
	TSubclassOf<class AActor> CannonTower;

};
