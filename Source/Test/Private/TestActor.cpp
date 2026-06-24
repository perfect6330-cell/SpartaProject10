#include "TestActor.h"
#include "../Test.h"
#include "Engine/Engine.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	const FString Message = TEXT("[Test Module] ATestActor BeginPlay - Test module is loaded and working.");
	UE_LOG(LogTest, Warning, TEXT("%s"), *Message);

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
	}
}
