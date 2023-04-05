// Code by Leonardo Paiva 


#include "RotatingLevelSection.h"

// Sets default values
ARotatingLevelSection::ARotatingLevelSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SectionMesh"));

	RootComponent = SectionMesh;

}

// Called when the game starts or when spawned
void ARotatingLevelSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingLevelSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldRotate)
	{
		FRotator TargetRotation = FRotator(GetActorRotation().Pitch + SectionRotationRate, GetActorRotation().Yaw, GetActorRotation().Roll);
		SetActorRotation(FMath::Lerp(GetActorRotation(), TargetRotation, 0.05f));
		if(GetActorRotation().Pitch == SectionRotationRate)
		{
			SectionRotationRate *= -1;
		}
	}

}
