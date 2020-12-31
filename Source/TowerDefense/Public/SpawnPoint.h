// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class TOWERDEFENSE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

	UPROPERTY(EditAnywhere, Category = "components")
	UStaticMeshComponent* SpawnMesh;

	UPROPERTY(EditAnywhere, Category = "cooldown")
	class UCooldown* cooldown;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy(std::string enemyType);

	//UPROPERTY(EditAnywhere, Category = "Cooldown")
	//float maxCooldown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "EnemySpawn")
	TSubclassOf<class AEnemy> EnemyClass;

	//uint32 countTicks;

//	inline bool IsNotCooldown();
//
//	inline void StartCooldown();
//
//private:
//	float actualCooldown;

};
