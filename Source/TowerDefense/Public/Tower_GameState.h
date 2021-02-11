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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealthPercentage();

	UFUNCTION(BlueprintPure, Category = "Stats")
	FText GetHealthText();

	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetGold();

	UFUNCTION(BlueprintPure, Category = "Stats")
	FText GetGoldText();

	bool isPaused;

private:
	class ATower_GameMode* GM;

};
