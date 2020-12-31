// #pragma optimize("", off)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Main_Menu_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AMain_Menu_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AMain_Menu_HUD();
	
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	UUserWidget * GetHUDWidget();

private:
	UPROPERTY(EditAnywhere, Category = "Stats")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UUserWidget * CurrentWidget;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void OpenOptions();

	UFUNCTION()
	void SetRes1080();

	UFUNCTION()
	void SetRes720();

	UFUNCTION()
	void SetRes480();

	UFUNCTION()
	void ReturnToMainMenu();
};
