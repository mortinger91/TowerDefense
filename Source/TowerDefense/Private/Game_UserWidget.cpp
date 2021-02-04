// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Game_UserWidget.h"

void UGame_UserWidget::NativeConstruct()
{
	FillAnimationsMap();

	// Call Blueprint Event Construct node
	Super::NativeConstruct();
}

void UGame_UserWidget::FillAnimationsMap()
{
	//AnimationsMap.Empty();
	
	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// Only want the properties that are widget animations
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene->GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

UWidgetAnimation* UGame_UserWidget::GetAnimationByName(FName AnimationName)
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
	if (WidgetAnim)
	{
		return *WidgetAnim;
	}
	return nullptr;
}

//bool UGame_UserWidget::PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
//{
//	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
//	if (WidgetAnim)
//	{
//		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
//		return true;
//	}
//	return false;
//}