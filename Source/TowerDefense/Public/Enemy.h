// Unreal Engine 4 Tower Defense

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

	bool iAmDestroyed;

protected:
	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystemComponent* Particles;

	float maxHealth;
	float gold;
	float health;
	
	// health bar widget component
	class UWidgetComponent* HealthWidgetComponent;
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	UUserWidget * HealthWidgetEnemy;

	class AEnemy_AIController* ai;

	class ATower_GameMode * GM;

};
