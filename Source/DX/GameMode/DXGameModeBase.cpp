// DXGameModeBase.cpp


#include "DXGameModeBase.h"



#include "Engine/Engine.h"

void ADXGameModeBase::StartPlay()
{
	Super::StartPlay();

	const FString Message = TEXT("[DX Primary Module] DXGameModeBase StartPlay - active GameMode confirmed.");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Yellow, Message);
	}
}
