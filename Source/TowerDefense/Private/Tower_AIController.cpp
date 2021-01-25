// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize("", off)

#include "Tower_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Tower.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cooldown.h"
#include "Kismet/GameplayStatics.h"

ATower_AIController::ATower_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	isActive = false;

	// component perception sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component"));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AISightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ATower_AIController::OnPawnDetected);
}

void ATower_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Perception Component not found!"))
	}
	
}

void ATower_AIController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	tower = Cast<ATower>(GetPawn());
	if (tower == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("No Tower Pawn found for %s!"), *this->GetName())
	}
}

// every frame the tower is checking the cooldown
// if the shot is not on cooldown, it checks if there are any detected pawns in the DetectedPawns TArray
// if a pawn is in the TArray, it shoots the the one with the oldest spawn time, then the shot is put on cooldown

// TO BE REFACTORED: when a pawn is added to the DetectedPawns TArray, it triggers an event
//                   when the shot has ended its cooldown it triggers an event
void ATower_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActive)
	{
		if (tower->cooldownShot->IsNotCooldown())
		{
			// shooting the enemy from the DetectedPawns TArray which has the lowest CreationTime
			if (DetectedPawns.Num() > 0)
			{
				int32 index = 0;
				float min = DetectedPawns[0]->CreationTime;
				for (int32 i = 0; i < DetectedPawns.Num(); ++i)
				{
					if (DetectedPawns[i]->CreationTime < min)
					{
						min = DetectedPawns[i]->CreationTime;
						index = i;
					}
				}
				Shoot(Cast<AEnemy>(DetectedPawns[index]));
				tower->cooldownShot->StartCooldown();
			}
		}
	}
}

// if an enemy enters the perception area, it is added in the DetectedPawns TArray
// when an enemy exits(exits or gets killed) the perception area, it is removed from the DetectedPawns TArray
void ATower_AIController::OnPawnDetected(const TArray<AActor*> &DetectedPawnsNow)
{
	if (isActive)
	{
		// aggiungere lock RAII da controllare prima di controllare DetectedPawns in tick
		// per evitare che tick parta mentre OnPawnDetected sta modificando l'array DetectedPawns
		if (DetectedPawnsNow.Last()->IsA(AEnemy::StaticClass()))
		{
			for (int i = 0; i < DetectedPawnsNow.Num(); ++i)
			{
				int32 index = -1;
				if (DetectedPawns.Find(DetectedPawnsNow[i], index))
				{
					DetectedPawns.RemoveAt(index);
					UE_LOG(LogActor, Warning, TEXT("Enemy detected: %s, Removed!"), *DetectedPawnsNow[i]->GetName())
				}
				else
				{
					DetectedPawns.Add(DetectedPawnsNow[i]);
					UE_LOG(LogActor, Warning, TEXT("Enemy detected: %s, Added!"), *DetectedPawnsNow[i]->GetName())
				}
			}
		}
	}
}

void ATower_AIController::Shoot(const AEnemy* enemyToShoot)
{
	if (tower->BulletClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = tower;
		SpawnParams.Instigator = tower;

		FTransform BulletSpawnTransform;
		
		// atomic
		FVector enemyLocation;
		{
			if (enemyToShoot == nullptr)
				return;
			enemyLocation = enemyToShoot->GetActorLocation();
		}

		float min = FVector::Dist(tower->sockets[0], enemyLocation);
		int32 socketIndex = 0;
		if (!(tower->sockets.Num() > 0))
		{
			UE_LOG(LogActor, Warning, TEXT("No Tower Sockets found for %s!"), *this->GetName())
		}
		for (int32 i = 0; i < tower->sockets.Num(); ++i)
		{
			if (FVector::Dist(tower->sockets[i], enemyLocation) < min)
			{
				min = FVector::Dist(tower->sockets[i], enemyLocation);
				socketIndex = i;
			}
		}
		
		BulletSpawnTransform.SetLocation(tower->sockets[socketIndex]);

		FVector fv = UKismetMathLibrary::InverseTransformLocation(BulletSpawnTransform, enemyLocation);
		FRotator fr = UKismetMathLibrary::Conv_VectorToRotator(fv);
		//BulletSpawnTransform.SetRotation(tower->GetActorRotation().Quaternion()); 
		BulletSpawnTransform.SetRotation(fr.Quaternion()); 
		BulletSpawnTransform.SetScale3D(FVector(0.8f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), tower->particlesShooting, BulletSpawnTransform, true, EPSCPoolMethod::None, true);
		GetWorld()->SpawnActor<ABullet>(tower->BulletClass, BulletSpawnTransform, SpawnParams);
	}
}

void ATower_AIController::ActivateAI()
{
	isActive = true;
}
