// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Ninja.h"
#include "GameplayStats.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ANinja::ANinja() : Super()
{
	HealthWidgetComponent->SetRelativeLocation(FVector(10.f, 0.f, 50.f));
	HealthWidgetComponent->SetDrawSize(FVector2D(60.f, 15.f));

	// STATS
	walkSpeed = Ninja::walkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	maxHealth = Ninja::baseHealth;
	gold = Ninja::gold;
}

void ANinja::BeginPlay()
{
	Super::BeginPlay();
}

void ANinja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
