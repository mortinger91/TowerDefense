// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Tower_AIController.generated.h"

UCLASS()
class TOWERDEFENSE_API ATower_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATower_AIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* MyPawn) override;

	virtual void Tick (float DeltaTime) override;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawnsNow);

	class UAISenseConfig_Sight* SightConfig;

	void ActivateAI();

private:
	class ATower* tower;

	TArray<class AEnemy*> DetectedEnemies;

	bool isActive;

	bool IsValid(class AEnemy* enemy) const;
};
