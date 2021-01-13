// Unreal Engine 4 Tower Defense
#pragma optimize("", off)

#include "Tower_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "CustomPawn.h"
//#include "GenericPlatform/ICursor.h"
#include "Tower.h"

//#include "Blueprint/UserWidget.h"
//#include "UObject/ConstructorHelpers.h"

ATower_PlayerController::ATower_PlayerController()
{
	//PrimaryActorTick.bCanEverTick = true;

	//GetHitResultUnderCursorByChannel
	
	//GetHitResultUnderCursorByChannel
	//PlayerInput->

	//static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/DefaultCursor"));
	//CursorWidgetClass = Cast<UUserWidget>(HealthBarObj.Class);
}

void ATower_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI data;
	data.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(data);
	bShowMouseCursor = true; 
	bEnableClickEvents = true; 
	bEnableMouseOverEvents = true;
	
	CurrentMouseCursor = EMouseCursor::Crosshairs;
	//SetMouseCursorWidget(GetMouseCursor(), CursorWidgetClass);

}

void ATower_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATower_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATower_PlayerController::GetFoundActors);

	InputComponent->BindAxis("MoveForward", this, &ATower_PlayerController::MoveForward); 
	InputComponent->BindAxis("MoveRight", this, &ATower_PlayerController::MoveRight);
	//InputComponent->BindAxis("Turn", this, &ATower_PlayerController::Turn);
	//InputComponent->BindAxis("LookUp", this, &ATower_PlayerController::LookUp);
}

void ATower_PlayerController::MoveForward(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->MoveForward(Amount);
}

void ATower_PlayerController::MoveRight(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->MoveRight(Amount);
}

void ATower_PlayerController::Turn(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->Turn(Amount);
}

void ATower_PlayerController::LookUp(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->LookUp(Amount);
}

void ATower_PlayerController::GetFoundActors()
{
	FHitResult Result;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
	if (ATower * tower = Cast<ATower>(Result.Actor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked Tower Actor: %s"), *tower->GetName()));
	}
}
