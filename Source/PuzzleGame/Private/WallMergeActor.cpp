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
		FVector CurrentLocation = CharCamera->GetComponentLocation();
		FVector SideVector = GetActorRightVector() * MovementVector.X;
		FVector TraceStart = CurrentLocation + SideVector * DistanceFromCenter;
		FVector TraceEnd = TraceStart + CharCamera->GetForwardVector() * TraceLenght;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
		
		FVector Normal = Hit.ImpactNormal;
		float Dot = FVector::DotProduct(Normal, GetActorForwardVector());
		float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(Dot));
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, true, -1, 0, 10);

		//we can make a function to handle this later
		if((ImpactAngle == 90.f || ImpactAngle == 180.f) && Hit.bBlockingHit == true)
		{
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
		else
		{	
			//Remember to set UseControllerRotationYaw to true
			AddControllerYawInput(-MovementVector.X);
		}

	}

}

/*TODO
the movement does not work pretty well when we change the direction after doing a curve
We can try using th set root component in runtime to do the rotation
Polish movement
shoot the right and left line traces
We can try attach this actor to the actor that is being hit so we can use the floating platforms
*/

