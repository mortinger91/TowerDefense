// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndPoint.generated.h"

UCLASS()
class TOWERDEFENSE_API AEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndPoint();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "components")
	UStaticMeshComponent* EndMesh;

	void SetEndPointPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void RemoveHealth(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit);

	class ATower_GameMode* GM;
};
