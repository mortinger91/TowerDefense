// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tower.generated.h"

UCLASS()
class TOWERDEFENSE_API ATower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATower();

	UPROPERTY(EditAnywhere, Category = "cooldown")
	class UCooldown* cooldownShot;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;

	// array of sockets, points from where to shoot
	TArray<FVector> sockets;

	float GetDamage(); //dipende dal livello

	int32 GetLevel(); // restituisce il livello
	 
	void LevelUp(); // aumenta di livello se possibile
	
	void Sell(); // aumenta di livello se possibile

	int32 GetGoldToUpgrade(); // restituisce i gold necessari al prossimo upgrade

	int32 GetGoldToSell();

	int32 GetGoldToBuild();

	FString towerType; // il tipo della torre

	void Activate();

	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATower_GameMode* GM;

	//UPROPERTY(EditAnywhere, Category = "components")
	//class UCapsuleComponent* TowerCapsule;

	UPROPERTY(EditAnywhere, Category = "components")
	UStaticMeshComponent* TowerMesh;

	float shiftSock;

	int32 level;

	float damage1;
	float damage2;
	float damage3;
	
	int32 goldToBuild;
	int32 goldToUpgrade1;
	int32 goldToUpgrade2;
	int32 goldToSell1;
	int32 goldToSell2;
	int32 goldToSell3;
	
};
