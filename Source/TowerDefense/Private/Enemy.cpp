// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy_AIController.h"
#include "Tower_GameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

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
	
	AIControllerClass = AEnemy_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_NavWalking;
	bUseControllerRotationYaw = false;

	// SERVONO?
		//GetCharacterMovement()->bUseRVOAvoidance = true;
		//GetCharacterMovement()->AvoidanceConsiderationRadius = 150.f;
		//GetCharacterMovement()->bCanWalkOffLedges = false;
		//GetCharacterMovement()->AvoidanceWeight = 0.5f;

	iAmDestroyed = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Particles->Deactivate();

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

void AEnemy::GetDamaged(float damage)
{
	health -= damage;
	Cast<UProgressBar>(HealthWidgetEnemy->GetWidgetFromName(FName("Health_Bar")))->SetPercent(health/maxHealth);

	if (health <= 0)
	{
		iAmDestroyed = true;

		GM->UpdateGold(gold);

		Destroy();
	}
}

void AEnemy::GetSlowed(float slow, float time)
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed - (walkSpeed*slow)/100;
	Particles->Activate();

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	//Binding the function with specific variables
	//TimerDel.BindUFunction(this, FName("RestoreMoveSpeed"), MyInt, MyFloat);
	TimerDel.BindUFunction(this, FName("RestoreMoveSpeed"));
	//Calling RestoreMoveSpeed after "time" seconds without looping
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, time, false);
}

void AEnemy::RestoreMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	Particles->Deactivate();
}


