// Unreal Engine 4 Tower Defense

#include "Tower.h"
#include "Cooldown.h"
#include "Components/StaticMeshComponent.h"
#include "Tower_GameMode.h"
#include "Tower_AIController.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "TowerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATower::ATower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// mesh component
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerMesh");
	SetRootComponent(TowerMesh);
	//TowerMesh->SetCollisionProfileName(TEXT("NoCollision"));

	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("capsule component");
	//SetRootComponent(CapsuleComponent);


	// cooldown component
	cooldownShot = CreateDefaultSubobject<UCooldown>("cooldown");

	AIControllerClass = ATower_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SetCanAffectNavigationGeneration(true, true);

	towerBase = nullptr;
	level = 1;
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Spawned tower: type: %s, name: %s"), *towerType, *this->GetName())

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower: Game Mode not found!"))
	}

	SpawnDefaultController();
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ATower::GetDamage() const
{
	switch (level)
	{
		case 1:
		{
			return damage1;
		}
		break;
		case 2:
		{
			return damage2;
		}
		break;
		case 3:
		{
			return damage3;
		}
		break;
	}
	return 0;
}

int32 ATower::GetLevel() const
{
	return level;
}

void ATower::LevelUp()
{
	bool playUpgradeParticles = false;
	switch (level)
	{
		case 1:
		{
			if (GM->GetAvailableGold(goldToUpgrade1))
			{
				level = 2;
				GM->UpdateGold(-goldToUpgrade1);
				playUpgradeParticles = true;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to upgrade tower: %s"), *GM->selectedTower->GetName()));
			}
		}
		break;
		case 2:
		{
			if (GM->GetAvailableGold(goldToUpgrade2))
			{
				level = 3;
				GM->UpdateGold(-goldToUpgrade2);
				playUpgradeParticles = true;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to upgrade tower: %s"), *GM->selectedTower->GetName()));
			}
		}
		break;
	}
	if (playUpgradeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), upgradeParticles, GetTransform(), true, EPSCPoolMethod::None, true);
	}
}

void ATower::Sell()
{
	GM->UpdateGold(GetGoldToSell());
	Destroy();
}

int32 ATower::GetGoldToUpgrade() const
{
	switch (level)
	{
		case 1:
		{
			return goldToUpgrade1;
		}
		break;
		case 2:
		{
			return goldToUpgrade2;
		}
		break;
	}
	return 0;
}

int32 ATower::GetGoldToSell() const
{
	switch (level)
	{
		case 1:
		{
			return goldToSell1;
		}
		break;
		case 2:
		{
			return goldToSell2;
		}
		break;
		case 3:
		{
			return goldToSell3;
		}
		break;
	}
	return 0;
}

int32 ATower::GetGoldToBuild() const
{
	return goldToBuild;
}

FString ATower::GetTowerType() const
{
	return towerType;
}

void ATower::Activate()
{
	towerPosition = GetActorLocation();
	Cast<ATower_AIController>(GetController())->ActivateAI();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), spawnParticles, GetTransform(), true, EPSCPoolMethod::None, true);
}

void ATower::GetActorEyesViewPoint(FVector & Location, FRotator & Rotation) const
{
	Location = TowerMesh->GetSocketLocation(FName("Head"));
	Rotation = GetActorRotation();
    //Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}

void ATower::SetCooldown(float cooldown)
{
	cooldownShot->maxCooldown = cooldown;
}

float ATower::GetAISightRadius() const
{
	return AISightRadius;
}

void ATower::SetTowerBase(ATowerBase* towerBaseToSet)
{
	towerBase = towerBaseToSet;
}

ATowerBase* ATower::GetTowerBase() const
{
	return towerBase;
}
