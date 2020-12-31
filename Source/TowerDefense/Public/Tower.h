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

	UPROPERTY(EditAnywhere, Category = "components")
	UStaticMeshComponent* TowerMesh;

	// array of sockets, points from where to shoot
	TArray<FVector> sockets;

	//UPROPERTY(EditAnywhere, Category = "Sockets")
	float shiftSock;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

	UPROPERTY(EditAnywhere, Category = "TowerDamage")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
