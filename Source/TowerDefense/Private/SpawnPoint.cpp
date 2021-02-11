// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "SpawnPoint.h"
#include "Ninja.h"
#include "Cooldown.h"
#include "GameplayStats.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Tower_GameMode.h"
#include "TimerManager.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	
	cooldown = CreateDefaultSubobject<UCooldown>("cooldown");

	cooldown->maxCooldown = Spawn::spawnCooldown;

	toWait = true;
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (NinjaClass != nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In SpawnPoint: NinjaClass not found!"))
	}

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In SpawnPoint: Game Mode not found!"))
	}
}

void ASpawnPoint::StopSpawning(float stopTime)
{
	toWait = true;
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	TimerDel.BindUFunction(this, FName("StartSpawning"));
	//Calling RestoreMoveSpeed after "time" seconds without looping
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, stopTime, false);
}

void ASpawnPoint::StartSpawning()
{
	GM->IncrementWave();
	toWait = false;
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!toWait)
	{
		if(cooldown->IsNotCooldown())
		{
			cooldown->StartCooldown();
			SpawnEnemy("Ninja");
		}
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

	if (enemyType == "Ninja")
	{
		GetWorld()->SpawnActor<ANinja>(NinjaClass, EnemySpawnTransform, SpawnParams);
	}
	//else if (enemyType == "...")

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Spawned an enemy!")));
}
