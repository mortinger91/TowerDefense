// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	class ATower* tower;

	TArray<AActor*> DetectedPawns;

	bool isActive;
};
