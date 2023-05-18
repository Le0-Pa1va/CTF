// Code by Leonardo Paiva 


#include "RevealingGate/RevealingGate.h"

// Sets default values
ARevealingGate::ARevealingGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GateRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GateRoot"));
	RootComponent = GateRoot;

	FirstPole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPole"));
	SecondPole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPole"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	FirstPole->SetupAttachment(RootComponent);
	SecondPole->SetupAttachment(RootComponent);
	BoxCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARevealingGate::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARevealingGate::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARevealingGate::OnOverlapEnd);
}

// Called every frame
void ARevealingGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector> ARevealingGate::GetPolesLocations()
{
	TArray<FVector> PolesLocations;

	PolesLocations.Add(FirstPole->GetComponentLocation());
	PolesLocations.Add(SecondPole->GetComponentLocation());

	return PolesLocations;
}

void ARevealingGate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA<APuzzleGameCharacter>())
	{
		if(bIsInFront)
		{
			SetCollidedCharater(true);
			bBackwardsCollision = false;
		}
		else
		{
			bBackwardsCollision = true;
		}
	}
}

void ARevealingGate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->IsA<APuzzleGameCharacter>())
	{
		SetCollidedCharater(false);
	}
}

void ARevealingGate::SetBoxCollisionResponse(bool bShouldBlock)
{
	if(bShouldBlock)
	{
		BoxCollision->SetCollisionProfileName("BlockAll");
	}
	else
	{
		BoxCollision->SetCollisionProfileName("OverlapAll");
	}
}

