// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "CannonTower.generated.h"

UCLASS()
class TOWERDEFENSE_API ACannonTower : public ATower
{
	GENERATED_BODY()

public:
	ACannonTower();

	void Activate() override;

	void Shoot(const AActor* enemyToShoot) override;

private:
	//// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	// array of sockets, points from where to shoot
	TArray<FVector> sockets;

	float shiftSock;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;
};
