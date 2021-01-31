// Unreal Engine 4 Tower Defense

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

	class UCooldown* cooldown;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy(FString enemyType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "EnemySpawn")
	TSubclassOf<class ANinja> NinjaClass;

};
