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

	// gold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 MaxGold;

	int32 AddGold(float gold);

	// end positions
	void AddToEndPositions(const FVector& fv);

	FVector GetEndPositions();

	// Returns the current playing state
	UFUNCTION(BlueprintPure, Category = "Game State")
	EGamePlayState GetCurrentState() const;

	// Sets a new playing state
	void SetCurrentState(EGamePlayState NewState);

private:
	std::vector<FVector> EndPointPosition;

	class ATower_GameState* GS;
	
	// Keeps track of the current playing state
	EGamePlayState CurrentState;

	// Handle any function calls that rely upon changing the playing state of our game
	void HandleNewState(EGamePlayState NewState);

};
