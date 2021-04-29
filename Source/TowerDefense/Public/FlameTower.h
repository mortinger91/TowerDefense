// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "FlameTower.generated.h"

UCLASS()
class TOWERDEFENSE_API AFlameTower : public ATower
{
	GENERATED_BODY()

public:
	AFlameTower();

	void Activate() override;

	void Shoot(const AActor* enemyToShoot) override;

private:
	void BeginPlay() override;

	FVector socket;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

};
