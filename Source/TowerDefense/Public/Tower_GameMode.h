// Unreal Engine 4 Tower Defense

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

	// STATS
	float MaxHealth;
	int32 MaxGold;

	// HEALTH
	float PreviousHealth;
	float HealthDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UCurveFloat* HealthCurve;
	float CurveFloatValue;
	float TimelineValue;
	class UTimelineComponent* MyTimeline;

	// HUD
	// player hud, update gold count when enemy dies
	class AGame_HUD * HudWidgetPlayer;
	UFUNCTION()
	void SetHealth();
	void Remove1Health();
	void UpdateGold(float gold);
	bool GoldAvailable(int32 GoldToCheck);
	// Handle any function calls that rely upon changing the playing state of our game
	void ChangeGamePlayState(EGamePlayState NewState);
	void PauseGame();
	void ToggleForward();

	// SELECTING TOWERS
	class ATower * selectedTower;
	void LevelUpSelectedTower();
	void SellSelectedTower();

	// SPAWNING TOWERS
	class ATower * spawnedTower;
	class ATowerBase* selectedBase;
	bool dragMode;
	void SpawnTower(FString towerType);
	void FinalizeTowerSpawn(); 

	// SPAWNING ENEMIES
	float healthMultiplier;
	void IncrementWave();
	int32 GetWave();

private:
	virtual void BeginPlay() override;

	virtual void Tick (float DeltaTime) override;

	int32 waveCount;
	class ASpawnPoint* spawnPoint;
	UFUNCTION()
	void StopBetweenWaves();

	class ATower_GameState* GS;
	
	// Keeps track of the current playing state
	EGamePlayState CurrentState;

	UFUNCTION()
	void GoToMainMenu();

	bool InitiateGameOver;

	UPROPERTY(EditAnywhere, Category = "Tower Types Blueprint")
	TSubclassOf<class ACannonTower> CannonTowerClass;

	UPROPERTY(EditAnywhere, Category = "Tower Types Blueprint")
	TSubclassOf<class AIceTower> IceTowerClass;

	void ShowUnusedTowerBases(bool toHide);

	bool normalSpeed;
};
