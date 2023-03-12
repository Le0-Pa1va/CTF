// Code by Leonardo Paiva 


#include "FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InterpComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovementComponent"));

	RootComponent = StaticMesh;

	for(int i = 0; i < 2; i++)
	{
		InterpComponent->AddControlPointPosition(FVector(0.f, 0.f, 0.f), true);
	}
}
