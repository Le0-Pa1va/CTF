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
	
	// CharCamera->SetRelativeLocation(FVector(-450.f, 0.f, 60.f)); // Position the camera
	RootComponent = ActorCenter;
	CharCamera->SetupAttachment(RootComponent);
	CharacterDecal->SetupAttachment(RootComponent);

	// Remember to set the decal size on the blueprint (i think the  size (300, 30, 30) should work)
	// the position in the tests that works fine is (X=1890.000000,Y=790.000000,Z=100.000000)
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

	UE_LOG(LogTemp, Error, TEXT("AAAAA---------------------------------------------"));
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

	// if(int32(SidewayTraceLenght) < int32(InitialDistance))
	// {
	// 	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y - (InitialDistance - CurrentDistance), GetActorLocation().Z));
	// }
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
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("entrou"));
	FString CurrentTime = FDateTime::Now().ToString();
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float AngleForwardHit = GetForwardImpactAngle(MovementVector.X);
		GetSidewaysImpact(MovementVector.X);

		UE_LOG(LogTemp, Warning, TEXT("AngleForwardHit %f"), AngleForwardHit);
		UE_LOG(LogTemp, Warning, TEXT("YawOnCorner %f"), YawOnCorner);
		UE_LOG(LogTemp, Error, TEXT("bHitOnFront %s"), bHitOnFront ? "t" : "f");
		UE_LOG(LogTemp, Error, TEXT("bHitOnSideways %s"), bHitOnSideways ? "t" : "f");
		UE_LOG(LogTemp, Warning, TEXT("bIsRotatingConcave %s"), bIsRotatingConcave ? "t" : "f");
		if((AngleForwardHit == 90.f || AngleForwardHit == 180.f) && bHitOnFront == true && bHitOnSideways == false)
		{
			bIsRotatingConcave = false;
			bIsRotatingConvex = false;
			AddMovementInput(GetActorRightVector(), MovementVector.X);
			/*TODO
			The problem is in the distance, so we have to debug this next part and improve the logic*/
			UE_LOG(LogTemp, Warning, TEXT("BBBBBBBBBBBBBBBBBBBBBBBB"));
    		UE_LOG(LogTemp, Log, TEXT("The current time is: %s"), *CurrentTime)
			if(int32(CurrentDistance) < int32(InitialDistance))
			{
				AddMovementInput(GetActorForwardVector(), -1);
			}
		}
		else if((bHitOnSideways == true || YawOnCorner != 0.f) && bIsRotatingConvex == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("AngleForwardHitAAAAAAAAA"));
			bIsRotatingConcave = true;
			
			if(GetRootComponent() != CharCamera)
			{
				SetPivotPoint(CharCamera);
			}
			else
			{
				// AddMovementInput(GetActorRightVector(), MovementVector.X);
				if(FMath::Abs(YawOnCorner) == 90.f || FMath::Abs(YawOnCorner) == 180.f)
				{
					bIsRotatingConcave = false;
					YawOnCorner = 0.f;
				}
				// TODO nunca vai cair aqui. Arrumar!!!!!!! 
				else if((AngleForwardHit == 90.f || AngleForwardHit == 180.f) && bHitOnFront == true && bHitOnSideways == false)
				{
					bIsRotatingConcave = false;
					bIsRotatingConvex = false;
					AddControllerYawInput(MovementVector.X);
					YawOnCorner = 0.1;
				}
				else
				{
					AddControllerYawInput(MovementVector.X);
					YawOnCorner = GetActorRotation().Yaw;
				}
			}
		}
		else if(bIsRotatingConcave == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("AQUI"));
			if(GetRootComponent() != CharacterDecal)
			{
				SetPivotPoint(CharacterDecal);
			}
			else
			{
				bIsRotatingConvex = true;
				//Remember to set UseControllerRotationYaw to true
				AddControllerYawInput(-MovementVector.X);
			}
		}

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
