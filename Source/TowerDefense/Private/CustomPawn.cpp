// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "CustomPawn.h"
#include "Bullet.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACustomPawn::ACustomPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	SetRootComponent(StaticMesh);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraArm->SetupAttachment(StaticMesh);
	//CameraArm->TargetArmLength = 400.f;
	CameraArm->TargetArmLength = 0.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	//non piu necessario perche la camera è attaccata al camera arm il quale è gia 500 unità dietro al player
	//Camera->SetRelativeLocation(FVector(-500.f, 0.f, 0.f));
	Camera->SetupAttachment(CameraArm);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogActor, Warning, TEXT("****************** Spawned %s ******************"), *this->GetName())
}

void ACustomPawn::MoveForward(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void ACustomPawn::MoveRight(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void ACustomPawn::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

void ACustomPawn::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ACustomPawn::Shoot()
{
	// sparare di fatto è spawnare l'actor "bullet", che ha gia una speed che gli viene applicata quando viene creato, dato che ha un component di tipo "UProjectileMovementComponent"

	// questo controllo va inserito per essere sicuri che una BulletClass è stata selezionata
	if (BulletClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FTransform BulletSpawnTransform;
		BulletSpawnTransform.SetLocation(GetActorForwardVector() * 500.f + GetActorLocation());
		BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
		BulletSpawnTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
	}
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
// Funzione usata per bindare gli input direttamente dentro il pawn, il che non richiede di definire un custom playercontroller
//void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACustomPawn::Shoot);
//
//	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomPawn::MoveForward); 
//	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomPawn::MoveRight);
//	PlayerInputComponent->BindAxis("Turn", this, &ACustomPawn::Turn);
//	PlayerInputComponent->BindAxis("LookUp", this, &ACustomPawn::LookUp);
//
//}
