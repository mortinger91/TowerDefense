// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Enemy.h"
#include "Cooldown.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpawnMesh");
	
	SetRootComponent(SpawnMesh);
	
	cooldown = CreateDefaultSubobject<UCooldown>("cooldown");

	//countTicks = 0;

	cooldown->maxCooldown = 1.f;
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnEnemy("soldier");
}

//inline bool ASpawnPoint::IsNotCooldown()
//{
//	if ((GetWorld()->GetRealTimeSeconds() - actualCooldown) > maxCooldown)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//inline void ASpawnPoint::StartCooldown()
//{
//	actualCooldown = GetWorld()->GetRealTimeSeconds();
//}



// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (countTicks++ % 10 == 0)
	//{
	//	countTicks = 0;
		if (cooldown->IsNotCooldown())
		{
			cooldown->StartCooldown();
			SpawnEnemy("soldier");
		}
	//}
}



void ASpawnPoint::SpawnEnemy(std::string enemyType)
{
	// sparare di fatto è spawnare l'actor "bullet", che ha gia una speed che gli viene applicata quando viene creato, dato che ha un component di tipo "UProjectileMovementComponent"

	// questo controllo va inserito per essere sicuri che una BulletClass è stata selezionata
	if (EnemyClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		//SpawnParams.Instigator = this;

		FTransform EnemySpawnTransform;
		EnemySpawnTransform.SetLocation(GetActorForwardVector() * 200.f + GetActorLocation());
		EnemySpawnTransform.SetRotation(GetActorRotation().Quaternion());
		EnemySpawnTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<AEnemy>(EnemyClass, EnemySpawnTransform, SpawnParams);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Spawned an enemy!")));
	}
}
