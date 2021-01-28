// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize("", off)


#include "Tower.h"
#include "Cooldown.h"
#include "Components/StaticMeshComponent.h"
#include "Tower_GameMode.h"
#include "GameplayStats.h"
#include "Tower_AIController.h"
//#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

// Sets default values
ATower::ATower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//TowerCapsule = CreateDefaultSubobject<UCapsuleComponent>("TowerCapsule");
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerMesh");
	//TowerMesh->SetupAttachment(TowerCapsule);
	//SetRootComponent(TowerCapsule);
	SetRootComponent(TowerMesh);

	// component cooldown
	cooldownShot = CreateDefaultSubobject<UCooldown>("cooldown");

	shiftSock = 35.f;


	// GAMEPLAY PARAMETERS
	towerType = "Cannon";
	level = 1;

	cooldownShot->maxCooldown = Cannon::shotCooldown;
	damage1 = Cannon::damage1;
	damage2 = Cannon::damage2;
	damage3 = Cannon::damage3;
	goldToBuild = Cannon::goldToBuild;
	goldToUpgrade1 = Cannon::goldToUpgrade1;
	goldToUpgrade2 = Cannon::goldToUpgrade2;
	goldToSell1 = Cannon::goldToSell1;
	goldToSell2 = Cannon::goldToSell2;
	goldToSell3 = Cannon::goldToSell3;
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower: Game Mode not found!"))
	}
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ATower::GetDamage()
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

int32 ATower::GetLevel()
{
	return level;
}

void ATower::LevelUp()
{
	switch (level)
	{
		case 1:
		{
			if (GM->GoldAvailable(goldToUpgrade1))
			{
				level = 2;
				GM->UpdateGold(-goldToUpgrade1);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to upgrade tower: %s"), *GM->selectedTower->GetName()));
			}
		}
		break;
		case 2:
		{
			if (GM->GoldAvailable(goldToUpgrade2))
			{
				level = 3;
				GM->UpdateGold(-goldToUpgrade2);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to upgrade tower: %s"), *GM->selectedTower->GetName()));
			}
		}
		break;
	}
}

void ATower::Sell()
{
	GM->UpdateGold(GetGoldToSell());
	Destroy();
}

int32 ATower::GetGoldToUpgrade()
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

int32 ATower::GetGoldToSell()
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

int32 ATower::GetGoldToBuild()
{
	return goldToBuild;
}

void ATower::Activate()
{
	// added ad hoc for the Tower_Merged_mesh
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket0")) + FVector(shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket1")) + FVector(-shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket2")) + FVector(0.f, -shiftSock, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket3")) + FVector(0.f, shiftSock, 0.f) );

	Cast<ATower_AIController>(GetController())->ActivateAI();
}

void ATower::GetActorEyesViewPoint(FVector & Location, FRotator & Rotation) const
{
	Location = TowerMesh->GetSocketLocation(FName("Head"));
	Rotation = GetActorRotation();
    //Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}


