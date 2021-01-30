// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Enemy.h"
#include "Cooldown.h"
#include "GameplayStats.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	cooldown = CreateDefaultSubobject<UCooldown>("cooldown");

	cooldown->maxCooldown = Spawn::spawnCooldown;

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyClass != nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In SpawnPoint: EnemyClass not found!"))
	}

}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (cooldown->IsNotCooldown())
	{
		cooldown->StartCooldown();
		SpawnEnemy("soldier");
	}
}

void ASpawnPoint::SpawnEnemy(FString enemyType)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;
	SpawnParams.Owner = this;

	FTransform EnemySpawnTransform;
	EnemySpawnTransform.SetLocation(GetActorForwardVector() * 200.f + GetActorLocation());
	EnemySpawnTransform.SetRotation(GetActorRotation().Quaternion());
	EnemySpawnTransform.SetScale3D(FVector(1.f));

	GetWorld()->SpawnActor<AEnemy>(EnemyClass, EnemySpawnTransform, SpawnParams);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Spawned an enemy!")));
}
