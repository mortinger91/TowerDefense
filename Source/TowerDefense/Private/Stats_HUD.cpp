// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Game_UserWidget.h"

AStats_HUD::AStats_HUD()
{
	static ConstructorHelpers::FClassFinder<UGame_UserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/New_Game_UI"));
	HUDWidgetClass = HealthBarObj.Class;
	
}

void AStats_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UGame_UserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AStats_HUD::DrawHUD()
{
	Super::DrawHUD();

}

//void AssignAnimations()
//{
//    UProperty* prop = GetClass()->PropertyLink;
//
//    // Run through all properties of this class to find any widget animations
//    while( prop != nullptr  )
//    {
//        // Only interested in object properties
//        if( prop->GetClass() == UObjectProperty::StaticClass() )
//        {
//            UObjectProperty* objectProp = Cast<UObjectProperty>(prop);
//
//            // Only want the properties that are widget animations
//            if( objectProp->PropertyClass == UWidgetAnimation::StaticClass() )
//            {
//                UObject* object = objectProp->GetObjectPropertyValue_InContainer( this );
//
//                UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);
//
//                if( widgetAnim != nullptr )
//                {
//                    // DO SOMETHING TO STORE OFF THE ANIM PTR HERE!
//                    // E.g. add to a TArray of some struct that holds info for each anim
//                }
//            }
//        }
//
//        prop = prop->PropertyLinkNext;
//    }
//}

// update the gold count in the player hud
void AStats_HUD::UpdateGoldText(int32 NewGoldCount)
{
	FString G = FString::FromInt(NewGoldCount);
	FText GText = FText::FromString(G);

	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("Gold_Text")))->SetText(GText);
}

//void AStats_HUD::UpdateHealthText(int32 NewHealth)
//{
//	FString G = FString::FromInt(NewHealth);
//	FText GText = FText::FromString(G);
//
//	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("Health_Text")))->SetText(GText);
//}

void AStats_HUD::GameOverMode()
{
	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("GAME_OVER_Text")))->SetVisibility(ESlateVisibility::Visible);

	UWidgetAnimation* anim = GetHUDWidget()->GetAnimationByName(FName("Fade"));
	GetHUDWidget()->PlayAnimation(anim);
}

UGame_UserWidget * AStats_HUD::GetHUDWidget()
{
	return CurrentWidget;
}
