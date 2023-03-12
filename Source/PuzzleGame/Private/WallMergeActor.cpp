// Code by Leonardo Paiva 


#include "WallMergeActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AWallMergeActor::AWallMergeActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorCenter = CreateDefaultSubobject<USceneComponent>(TEXT("ActorCenter"));
	CharacterDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	CharCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovComp"));

	RootComponent = ActorCenter;
	CharCamera->SetupAttachment(RootComponent);
	CharacterDecal->SetupAttachment(RootComponent);

	// Remember to set the decal size on the blueprint (i think the  size (300, 30, 30) should work)
}

// Called when the game starts or when spawned
void AWallMergeActor::BeginPlay()
{
	Super::BeginPlay();

	bHitOnFront = true;
	bIsRotatingConcave = false;
	bIsRotatingConvex = false;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FHitResult Hit;
	FVector TraceStart = CharCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + CharCamera->GetForwardVector() * TraceLenght;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if(Hit.bBlockingHit == true)
	{
		InitialDistance = FVector::Distance(Hit.ImpactPoint, TraceStart);
		CurrentDistance = InitialDistance;
		bHitOnFront = true;
	}
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
		float AngleForwardHit = GetForwardImpactAngle(MovementVector.X);
		GetSidewaysImpact(MovementVector.X);

		MovementOnTheWall(MovementVector.X, AngleForwardHit);
	}

}

float AWallMergeActor::GetForwardImpactAngle(float XMovementValue)
{
		FHitResult Hit;
		FVector CurrentLocation = CharCamera->GetComponentLocation();
		FVector SideVector = GetActorRightVector() * XMovementValue;
		FVector TraceStart = CurrentLocation + SideVector * DistanceFromCenter;
		FVector TraceEnd = TraceStart + CharCamera->GetForwardVector() * TraceLenght;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

		if(Hit.bBlockingHit == true)
		{
			CurrentDistance = FVector::Distance(Hit.ImpactPoint, TraceStart);
			if(bIsRotatingConvex == false && bIsRotatingConcave == false)
			{
				SidewayTraceLenght = FVector::Distance(Hit.ImpactPoint, TraceStart);
			}
			bHitOnFront = true;
		}
		else
		{
			bHitOnFront = false;
		}
		
		FVector Normal = Hit.ImpactNormal;
		float Dot = FVector::DotProduct(Normal, GetActorForwardVector());
		float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(Dot));

		return ImpactAngle;
}


void AWallMergeActor::GetSidewaysImpact(float XMovementValue)
{
		FHitResult Hit;
		FVector CurrentLocation = CharCamera->GetComponentLocation();
		FVector SideVector = GetActorRightVector() * XMovementValue;
		FVector TraceStart = CurrentLocation + SideVector;
		FVector TraceEnd = TraceStart + SideVector * SidewayTraceLenght; //TODO mudar tamanho aqui

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);

		if(Hit.bBlockingHit == true)
		{
			if(bHitOnSideways == false)
			{
				bHitOnSideways = true;
			}
		}
		else
		{
			bHitOnSideways = false;
		}
}

void AWallMergeActor::SetPivotPoint(USceneComponent* NewPivotPoint)
{
	if(NewPivotPoint)
	{
		NewPivotPoint->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		SetRootComponent(NewPivotPoint);
		PawnMovement->SetUpdatedComponent(RootComponent);
	}

	if(NewPivotPoint == CharCamera && CharacterDecal)
	{
		CharacterDecal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
	else if(CharCamera && CharacterDecal)
	{
		CharCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
	ActorCenter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AWallMergeActor::MovementOnTheWall(float XMovementValue, float AngleForwardHit)
{
	if((AngleForwardHit == 90.f || AngleForwardHit == 180.f) && bHitOnFront == true && bHitOnSideways == false)
		{
			YawOnCorner = 0.f;
			if(bIsRotatingConcave == true)
			{
				bIsRotatingConcave = false;
			}
			else if(bIsRotatingConvex == true)
			{
				bIsRotatingConvex = false;
			}

			AddMovementInput(GetActorRightVector(), XMovementValue);

			if(CurrentDistance < InitialDistance)
			{
				AddMovementInput(GetActorForwardVector(), -1);
			}
		}

		else if((bHitOnSideways == true || YawOnCorner != 0.f) && bIsRotatingConvex == false)
		{
			bIsRotatingConcave = true;
			
			if(GetRootComponent() != CharCamera)
			{
				SetPivotPoint(CharCamera);
			}
			else
			{
				if(FMath::Abs(YawOnCorner) == 90.f || FMath::Abs(YawOnCorner) == 180.f)
				{
					bIsRotatingConcave = false;
					YawOnCorner = 0.f;
				}
				else
				{
					AddControllerYawInput(XMovementValue);
					YawOnCorner = GetActorRotation().Yaw;
				}
			}
		}

		else if(bIsRotatingConcave == false)
		{
			if(GetRootComponent() != CharacterDecal)
			{
				SetPivotPoint(CharacterDecal);
			}
			else
			{
				bIsRotatingConvex = true;
				//Remember to set UseControllerRotationYaw to true
				AddControllerYawInput(-XMovementValue);
			}
		}
}
