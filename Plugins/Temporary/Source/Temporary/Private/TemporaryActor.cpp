#include "TemporaryActor.h"
#include "Temporary.h"
#include "Engine/Engine.h"

ATemporaryActor::ATemporaryActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATemporaryActor::BeginPlay()
{
	Super::BeginPlay();

	const FString Message = TEXT("[Temporary Plugin] ATemporaryActor BeginPlay - plugin module is working.");
	UE_LOG(LogTemporary, Warning, TEXT("%s"), *Message);

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
	}
}
