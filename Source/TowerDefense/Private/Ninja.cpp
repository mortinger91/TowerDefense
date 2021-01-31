// Unreal Engine 4 Tower Defense

#include "Ninja.h"
#include "GameplayStats.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANinja::ANinja() : Super()
{
	HealthWidgetComponent->SetRelativeLocation(FVector(10.f, 0.f, 50.f));
	HealthWidgetComponent->SetDrawSize(FVector2D(60.f, 15.f));

	// STATS
	GetCharacterMovement()->MaxWalkSpeed = Ninja::walkSpeed;
	maxHealth = Ninja::baseHealth;
	gold = Ninja::gold;
}

void ANinja::BeginPlay()
{
	Super::BeginPlay();

	// ogni 30 secondi il moltiplicatore aumenta di 0.5
	float healthMultiplier = 1.f + int32(GetWorld()->GetRealTimeSeconds() / 30) * 0.5f;

	maxHealth *= healthMultiplier;
	UE_LOG(LogActor, Warning, TEXT("Spawned Enemy with health: %f at time: %f"), maxHealth, GetWorld()->GetRealTimeSeconds())
	health = maxHealth;
}

void ANinja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
