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

	// array of sockets, points from where to shoot
	TArray<FVector> sockets;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* particlesShooting;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "TowerDamage")
	float damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "components")
	UStaticMeshComponent* TowerMesh;

	//UPROPERTY(EditAnywhere, Category = "Sockets")
	float shiftSock;

	UFUNCTION(BlueprintCallable)
	void OnClickedAction(AActor* TouchedActor, FKey ButtonPressed);

};
