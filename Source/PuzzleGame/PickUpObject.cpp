// Code by Leonardo Paiva 


#include "PickUpObject.h"
#include "GameFramework/Character.h"

// Sets default values
APickUpObject::APickUpObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	bIsPickedUp = false;
}

// Called when the game starts or when spawned
void APickUpObject::BeginPlay()
{
	Super::BeginPlay();
	
	// MainCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	// CharacterCamera = MainCharacter->FindComponentByClass<UCameraComponent>();
}

// Called every frame
void APickUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

