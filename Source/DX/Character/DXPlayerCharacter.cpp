// DXPlayerCharacter.cpp

#include "DXPlayerCharacter.h"

#include "TestActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

ADXPlayerCharacter::ADXPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ADXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ---------------------------------------------------------------------
	// Module interaction verification
	// DX primary module references the Test module and spawns ATestActor.
	// This is intentionally placed in the actual player character BeginPlay
	// so it runs when BP_DXCharacter is used as the Default Pawn Class.
	// ---------------------------------------------------------------------
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.0f + FVector(0.0f, 0.0f, 80.0f);
		const FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATestActor* SpawnedTestActor = World->SpawnActor<ATestActor>(
			ATestActor::StaticClass(),
			SpawnLocation,
			SpawnRotation,
			SpawnParameters
		);

		const bool bSpawnSucceeded = IsValid(SpawnedTestActor);
		const FString Message = bSpawnSucceeded
			? TEXT("[DX Primary Module] SpawnActor<ATestActor> succeeded from DXPlayerCharacter.")
			: TEXT("[DX Primary Module] SpawnActor<ATestActor> failed from DXPlayerCharacter.");

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				8.0f,
				bSpawnSucceeded ? FColor::Cyan : FColor::Red,
				Message
			);
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !InputMappingContext)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ADXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADXPlayerCharacter::HandleMoveInput);
	}

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADXPlayerCharacter::HandleLookInput);
	}

	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

void ADXPlayerCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
	const FVector2D MovementVector = InValue.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ADXPlayerCharacter::HandleLookInput(const FInputActionValue& InValue)
{
	const FVector2D LookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
