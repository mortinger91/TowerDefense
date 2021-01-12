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


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Particles = CreateDefaultSubobject<UParticleSystemComponent>("Particles");
	FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, false);
	Particles->AttachToComponent(RootComponent, rules);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Health Widget");
	HealthWidgetComponent ->AttachToComponent(RootComponent, rules);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//this->bUseControllerRotationYaw = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 0.5f;

	maxHealth = 100.f;
	health = maxHealth;
	gold = 1.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Enemy: Game Mode not found!"))
	}

	HealthWidgetEnemy = Cast<UUserWidget>(HealthWidgetComponent ->GetUserWidgetObject());
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
	//Move();

	ai->GoToRandomEndPoint();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AEnemy::Move()
//{
//	FVector dest;
//	GM->GetEndPositions(dest);
//	//UE_LOG(LogActor, Warning, TEXT("Moving Enemy to coordinates: x: %f, y: %f, z: %f"), dest.X, dest.Y, dest.Z)
//	ai->MoveToLocation(dest, -1, false, true, false, false, NULL, true);
//}

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
		GM->AddGold(gold);

		Destroy();

		//if (DeathActor != nullptr)
		//{
		//	FActorSpawnParameters SpawnParams;
		//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//	SpawnParams.bNoFail = true;
		//	SpawnParams.Owner = this;
		//	SpawnParams.Instigator = this;

		//	FTransform BulletSpawnTransform;
		//	BulletSpawnTransform.SetLocation(GetActorForwardVector() + GetActorLocation());
		//	BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
		//	BulletSpawnTransform.SetScale3D(FVector(1.f));

		//	GetWorld()->SpawnActor<AEnemyDeath>(DeathActor, BulletSpawnTransform, SpawnParams);
		//}
	}
}

