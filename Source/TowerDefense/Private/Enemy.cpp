// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy_AIController.h"
#include "AIController.h"
#include "Tower_GameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "GameplayStats.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Particles = CreateDefaultSubobject<UParticleSystemComponent>("Particles");
	FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, false);
	Particles->AttachToComponent(RootComponent, rules);

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/Enemy_Health_UI"));
	HUDWidgetClass = HealthBarObj.Class;
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Health Widget");
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->AttachToComponent(RootComponent, rules);
	HealthWidgetComponent->SetWidgetClass(HUDWidgetClass);

	HealthWidgetComponent->SetRelativeLocation(FVector(10.f, 0.f, 50.f));
	HealthWidgetComponent->SetDrawSize(FVector2D(60.f, 15.f));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	iAmDestroyed = false;

	// SERVONO?
		GetCharacterMovement()->bUseRVOAvoidance = true;
		GetCharacterMovement()->AvoidanceWeight = 0.5f;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		//GetCharacterMovement()->bUseControllerDesiredRotation = true;
		//this->bUseControllerRotationYaw = false;
		//GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);

	// STATS
	GetCharacterMovement()->MaxWalkSpeed = Enemy::walkSpeed;
	maxHealth = Enemy::baseHealth;
	gold = Enemy::gold;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// ogni 30 secondi il moltiplicatore aumenta di 0.5
	float healthMultiplier = 1.f + int32(GetWorld()->GetRealTimeSeconds() / 30) * 0.5f;

	maxHealth *= healthMultiplier;
	UE_LOG(LogActor, Warning, TEXT("Spawned Enemy with health: %f at time: %f"), maxHealth, GetWorld()->GetRealTimeSeconds())
	health = maxHealth;

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Enemy: Game Mode not found!"))
	}

	HealthWidgetEnemy = Cast<UUserWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (HealthWidgetEnemy == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Enemy: HealthWidgetEnemy not found!"))
	}
	Cast<UProgressBar>(HealthWidgetEnemy->GetWidgetFromName(FName("Health_Bar")))->SetPercent(1.f);

	ai = Cast<AEnemy_AIController>(GetController());
	if (ai == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Enemy: AEnemy_AIController not found!"))
	}

	ai->GoToRandomEndPoint();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::UpdateEnemyHealth()
{
	float perc = health/maxHealth;
	Cast<UProgressBar>(HealthWidgetEnemy->GetWidgetFromName(FName("Health_Bar")))->SetPercent(perc);
}

void AEnemy::GetDamaged(float damage)
{
	health -= damage;
	UpdateEnemyHealth();

	if (health <= 0)
	{
		iAmDestroyed = true;

		GM->UpdateGold(gold);

		Destroy();
	}
}


