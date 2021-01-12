// Unreal Engine 4 Tower Defense


#include "Tower_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

ATower_PlayerController::ATower_PlayerController()
{
	//PrimaryActorTick.bCanEverTick = true;

}

void ATower_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATower_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
//void ATower_PlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	PlayerInputComponent->BindAxis("MoveForward", this, &ACustomPawn::MoveForward); 
//	PlayerInputComponent->BindAxis("MoveRight", this, &ACustomPawn::MoveRight);
//	PlayerInputComponent->BindAxis("Turn", this, &ACustomPawn::Turn);
//	PlayerInputComponent->BindAxis("LookUp", this, &ACustomPawn::LookUp);
//
//}

//void ATower_PlayerController::MoveForward(float Amount)
//{
//	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
//}
//
//void ATower_PlayerController::MoveRight(float Amount)
//{
//	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
//}
//
//void ATower_PlayerController::Turn(float Amount)
//{
//	AddControllerYawInput(Amount);
//}
//
//void ATower_PlayerController::LookUp(float Amount)
//{
//	AddControllerPitchInput(Amount);
//}