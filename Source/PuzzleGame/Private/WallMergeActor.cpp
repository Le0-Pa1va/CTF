// Code by Leonardo Paiva 


#include "WallMergeActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AWallMergeActor::AWallMergeActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CharacterDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	CharCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovComp"));

	RootComponent = CharacterDecal;
	CharCamera->SetupAttachment(CharacterDecal);
	BoxCollision->SetupAttachment(CharacterDecal);
	CharCamera->SetRelativeLocation(FVector(-450.f, 0.f, 60.f)); // Position the camera

	// Remember to set the decal size on the blueprint (i think the  size (300, 30, 30) should work)
}

// Called when the game starts or when spawned
void AWallMergeActor::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	bIsRotating = false;
	NewRotationAxis = 0.f;
}

// Called every frame
void AWallMergeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWallMergeActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWallMergeActor::Move);
	}
}

void AWallMergeActor::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FHitResult Hit;
		TraceDistanceToCenter = TraceDistanceToCenter * MovementVector.X;
		FVector CurrentLocation = CharCamera->GetComponentLocation();
		FVector TraceStart = FVector(CurrentLocation.X, CurrentLocation.Y + TraceDistanceToCenter, CurrentLocation.Z);
		FVector TraceEnd = TraceStart + CharCamera->GetForwardVector() * TraceLenght;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, true, -1, 0, 10);

		if(!Hit.bBlockingHit && bIsRotating == false)
		{
			//Remember to set UseControllerRotationYaw to true
			AddControllerYawInput(-MovementVector.X);
		}
		else
		{
			// adding sideways movement
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}

	}
}

/*TODO
investigate why backwards the input does not work very well
We can try using th set root component in runtime to do the rotation
Polish movement
shoot the right and left line traces
*/

