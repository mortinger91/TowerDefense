// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetDamaged(float damage);

protected:
	//void Move();

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystemComponent* Particles;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float maxHealth;

	float health;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float gold;
	
	UPROPERTY(EditAnywhere, Category = "HealthWidget")
	class UWidgetComponent* HealthWidgetComponent;

	// own health bar
	UUserWidget * HealthWidgetEnemy;

	class AEnemy_AIController* ai;

	class ATower_GameMode * GM;

	void UpdateEnemyHealth();
};
