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

	FirstPole->SetupAttachment(RootComponent);
	SecondPole->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARevealingGate::BeginPlay()
{
	Super::BeginPlay();
	
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

