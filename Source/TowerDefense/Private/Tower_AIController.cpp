// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Tower_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.h"
#include "Tower.h"
#include "Cooldown.h"
#include "GameplayStats.h"
#include <mutex>

std::mutex miamutex;

ATower_AIController::ATower_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	isActive = false;

	// component perception sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component"));
	SightConfig->SightRadius = 1200.f;
	// not overlapping to avoid placing a tower and having an enemy on the edges of both
	SightConfig->LoseSightRadius = 1200.f + 100.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->SetMaxAge(0.f);
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

bool ATower_AIController::IsValid(AEnemy* enemy)
{
	if (enemy->iAmDestroyed)
	{
		return false;
	}
	// same as (enemy == nullptr)
	if(!enemy)
	{
		return false;
	}
	if(!enemy->IsValidLowLevel()) 
	{
		return false;
	}
	if(enemy->IsPendingKill()) 
	{
		return false;
	}
	return true;
}

// every frame the tower is checking the cooldown
// if the shot is not on cooldown, it checks if there are any detected pawns in the DetectedEnemies TArray
// if a pawn is in the TArray, it shoots the the older one in range, then the shot is put on cooldown
void ATower_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActive && tower->cooldownShot->IsNotCooldown())
	{
		std::lock_guard<std::mutex> lock(miamutex);
		int32 i = 0;
		while (i < DetectedEnemies.Num())
		{
			if (!IsValid(DetectedEnemies[i]))
			{
				DetectedEnemies.RemoveAt(i);
			}
			else 
			{
				FVector enemyLocation = DetectedEnemies[i]->GetActorLocation();
				float vec =  FVector::Dist(tower->towerPosition, enemyLocation);
				if ( vec > tower->GetAISightRadius() )
				{
					++i;
				}
				else
				{
					tower->Shoot(DetectedEnemies[i]);
					tower->cooldownShot->StartCooldown();
					break;
				}
			}
		}
	}
}

// enemies are removed only when nullptr, before shooting the distance between the tower and the enemy is checked
void ATower_AIController::OnPawnDetected(const TArray<AActor*> &DetectedPawnsNow)
{
	if (isActive)
	{
		std::lock_guard<std::mutex> lock(miamutex);
		for (int i = 0; i < DetectedPawnsNow.Num(); ++i)
		{
			if (DetectedPawnsNow[i]->IsA(AEnemy::StaticClass()))
			{
				int32 index = -1;
				if (!(DetectedEnemies.Find(Cast<AEnemy>(DetectedPawnsNow[i]), index)))
				{
					DetectedEnemies.Add(Cast<AEnemy>(DetectedPawnsNow[i]));
					UE_LOG(LogActor, Warning, TEXT("Enemy %s Added!"), *DetectedPawnsNow[i]->GetName())
				}
				else
				{
					DetectedEnemies.RemoveAt(index);
					UE_LOG(LogActor, Warning, TEXT("Enemy %s Removed!"), *DetectedPawnsNow[i]->GetName())
				}
			}
		}
	}
}

void ATower_AIController::ActivateAI()
{
	isActive = true;
}
