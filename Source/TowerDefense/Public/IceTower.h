

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "IceTower.generated.h"

UCLASS()
class TOWERDEFENSE_API AIceTower : public ATower
{
	GENERATED_BODY()
	
public:
	AIceTower();

	void Activate() override;

	void Shoot(const AActor* enemyToShoot) override;

	float GetSlowTime();

private:
	virtual void BeginPlay() override;

	FVector socket;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class AIceBullet> BulletClass;

	float slowTime1;
	float slowTime2;
	float slowTime3;
};