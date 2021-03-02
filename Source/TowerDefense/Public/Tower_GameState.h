// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Tower_GameState.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATower_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATower_GameState();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	// function that is still called in GAME_UI blueprint, port it to C++ adjusting floating values on each tick according to GM->UCurveFloat* HealthCurve
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealthPercentage() const;
	FText GetHealthText() const;

	int32 GetGold() const;
	FText GetGoldText() const;

	bool isPaused;

private:
	class ATower_GameMode* GM;

};
