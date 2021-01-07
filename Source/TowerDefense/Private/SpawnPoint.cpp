// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Enemy.h"
#include "Cooldown.h"
//#include "Tower_GameMode.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpawnMesh");
	
	//SetRootComponent(SpawnMesh);
	
	cooldown = CreateDefaultSubobject<UCooldown>("cooldown");

	cooldown->maxCooldown = 1.f;

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	//GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	//if (GM == nullptr)
	//{
	//	UE_LOG(LogActor, Warning, TEXT("In SpawnPoint: Game Mode not found!"))
	//}

}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FVector temp;
	//if (GM->GetEndPositions(temp) && cooldown->IsNotCooldown())
	if (cooldown->IsNotCooldown())
	{
		cooldown->StartCooldown();
		SpawnEnemy("soldier");
	}
}

void ASpawnPoint::SpawnEnemy(FString enemyType)
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
