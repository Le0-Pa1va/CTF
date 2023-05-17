// Code by Leonardo Paiva 


#include "RevealingGate/RevealingObject.h"

// Sets default values
ARevealingObject::ARevealingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;
}

// Called when the game starts or when spawned
void ARevealingObject::BeginPlay()
{
	Super::BeginPlay();
	
	if(MaskingMaterialInstance && RevealingGate)
	{
		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(MaskingMaterialInstance, this);
		FVector FirstPoleLocation = RevealingGate->GetPolesLocations()[0];
		FVector SecondPoleLocation = RevealingGate->GetPolesLocations()[1];

		DynMaterial->SetVectorParameterValue("Pole1", FLinearColor(FirstPoleLocation.X, FirstPoleLocation.Y, FirstPoleLocation.Z, 1.f));
		DynMaterial->SetVectorParameterValue("Pole2", FLinearColor(SecondPoleLocation.X, SecondPoleLocation.Y, SecondPoleLocation.Z, 1.f));
		DynMaterial->SetScalarParameterValue("ShowDefault", ShouldStartShowingScalar);

		ObjectMesh->SetMaterial(0.f, DynMaterial);
	}
}

// Called every frame
void ARevealingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

