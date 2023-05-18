// Code by Leonardo Paiva 


#include "RevealingGate/RevealingObject.h"
#include "Kismet/GameplayStatics.h"

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
		OpacityAfterCollision = ShouldStartShowingScalar;
		bCollidedGate = false;
		DynMaterial = UMaterialInstanceDynamic::Create(MaskingMaterialInstance, this);
		FVector FirstPoleLocation = RevealingGate->GetPolesLocations()[0];
		FVector SecondPoleLocation = RevealingGate->GetPolesLocations()[1];

		DynMaterial->SetVectorParameterValue("Pole1", FLinearColor(FirstPoleLocation.X, FirstPoleLocation.Y, FirstPoleLocation.Z, 1.f));
		DynMaterial->SetVectorParameterValue("Pole2", FLinearColor(SecondPoleLocation.X, SecondPoleLocation.Y, SecondPoleLocation.Z, 1.f));
		SetDynMaterialParam("ShowDefault", ShouldStartShowingScalar);
		SetDynMaterialParam("AfterCollision", 3.f);
		ObjectMesh->SetMaterial(0.f, DynMaterial);
		MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
}

// Called every frame
void ARevealingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandlePlayerGatePassing();
}


void ARevealingObject::HandlePlayerGatePassing()
{
	if(RevealingGate && MainCharacter)
	{
		FVector RelativeLocation = MainCharacter->GetActorLocation() - RevealingGate->GetActorLocation();
		RevealingGate->SetIsInFront(RelativeLocation.Y > 0.f);
		
		if(RevealingGate->GetIsInFront())
		{
			RevealingGate->SetBoxCollisionResponse(false);
			if(RevealingGate->GetCollidedCharater())
			{
				SetDynMaterialParam("AfterCollision", OpacityAfterCollision);
				ShouldStartShowingScalar = !OpacityAfterCollision;
				bCollidedGate = true;
			}
			else
			{
				SetDynMaterialParam("AfterCollision", 3.f);

				if(bCollidedGate && !RevealingGate->GetBackwardsCollision())
				{
					SetDynMaterialParam("ShowDefault", ShouldStartShowingScalar);
					OpacityAfterCollision = ShouldStartShowingScalar;
				}
				else //if(bCollidedGate && RevealingGate->GetBackwardsCollision())
				{
					SetDynMaterialParam("ShowDefault", OpacityAfterCollision);
				}
				// else if(!bCollidedGate && RevealingGate->GetBackwardsCollision())
				// {
				// 	// SetDynMaterialParam("AfterCollision", OpacityAfterCollision);
				// 	ShouldStartShowingScalar = !ShouldStartShowingScalar;
				// 	OpacityAfterCollision = !OpacityAfterCollision;
				// }
				bCollidedGate = false;
			}
		}

		else
		{
			if(bCollidedGate)
			{
				SetDynMaterialParam("AfterCollision", OpacityAfterCollision);
				SetCollision(OpacityAfterCollision);
				RevealingGate->SetBoxCollisionResponse(false);
			}
			else //if(!RevealingGate->GetBackwardsCollision())
			{
				SetDynMaterialParam("AfterCollision", !OpacityAfterCollision);
				SetCollision(!OpacityAfterCollision);
				RevealingGate->SetBoxCollisionResponse(true);
			}
		}
	}
}

void ARevealingObject::SetCollision(float Opacity)
{
	if(Opacity == 1)
	{
		ObjectMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		ObjectMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}

void ARevealingObject::SetDynMaterialParam(FName ParamName, float ParamValue)
{
	DynMaterial->SetScalarParameterValue(ParamName, ParamValue);
}
