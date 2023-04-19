// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PickUpObject.h"
#include "Public/RotateSectionButton.h"


//////////////////////////////////////////////////////////////////////////
// APuzzleGameCharacter

APuzzleGameCharacter::APuzzleGameCharacter()
{	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	bIsGrabbingObject = false;

}

void APuzzleGameCharacter::Tick(float DeltaTime)
{
	if(bIsGrabbingObject == true)
	{
		PhysicsHandle->SetTargetLocationAndRotation(FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * TraceLenght, FirstPersonCameraComponent->GetComponentRotation());
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void APuzzleGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Look);

		//PickUp
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::PickUp);

		//Rewind
		EnhancedInputComponent->BindAction(RewindAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Rewind);
	}
}


void APuzzleGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APuzzleGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APuzzleGameCharacter::PickUp(const FInputActionValue& Value)
{
	
	FHitResult Hit;
	FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
	FVector TraceEnd = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * TraceLenght;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// LineTrace
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	bool bHitSomething = Hit.bBlockingHit;

	if (Hit.bBlockingHit && !bIsGrabbingObject)
	{
		AActor* HitActor = Hit.GetActor();
		bool bIsValidActor = IsValid(HitActor);
		bool bPickUpable =  HitActor->GetClass()->IsChildOf<APickUpObject>();
		
		if(bIsValidActor == true && bPickUpable == true)
		{
			UPrimitiveComponent* HitComponent = Hit.GetComponent();
			FVector HitComponentLocation = HitComponent->GetComponentLocation();
			FRotator HitComponentRotation = HitComponent->GetComponentRotation();
			PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitComponentLocation, HitComponentRotation);

			bIsGrabbingObject = true;
			CheckGrabbedObject(HitActor);
		}
		else
		{
			CheckGrabbedObject(HitActor);
		}
	}
	
	else if(bIsGrabbingObject)
	{
		PhysicsHandle->ReleaseComponent();
		bIsGrabbingObject = false;
		
		if(ObjectToRewind)
		{
			ObjectToRewind->SetFoundFloor(false);
			ObjectToRewind->SetRecording(false);
		}
	}
}

void APuzzleGameCharacter::Rewind(const FInputActionValue& Value)
{
	if(
		bIsGrabbingObject == false &&
		ObjectToRewind &&
		ObjectToRewind->GetRewinding() == false &&
		ObjectToRewind->GetFoundFloor() == true
	)
	{
		ObjectToRewind->SetRewinding(true);
	}
}

void APuzzleGameCharacter::CheckGrabbedObject(AActor* GrabbedActor)
{
	ARotateSectionButton* RotateSectionButton = Cast<ARotateSectionButton>(GrabbedActor);
	if(RotateSectionButton)
	{
		RotateSectionButton->PressButton();
		return;
	}
	
	//this should be always the last one because we can't lose the reference
	ObjectToRewind = Cast<ARewindObject>(GrabbedActor);
	if(ObjectToRewind && bIsGrabbingObject == true)
	{
		ObjectToRewind->SetShouldDetach(true);
		ObjectToRewind->SetRecording(true);
	}
}
