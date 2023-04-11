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
	StartRotation = GetActorRotation();
}

// Called every frame
void ARotatingLevelSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRotate)
	{
		RotateSection(DeltaTime);
	}
}

void ARotatingLevelSection::RotateSection(float DeltaSeconds)
{
	// Lerp to target rotation or reverse rotation
	if (!bReverseRotation)
	{
		LerpAlpha += DeltaSeconds * LerpVelocity;
		if (LerpAlpha >= 1.f)
		{
			LerpAlpha = 1.f;
			bReverseRotation = true;
			bShouldRotate = false;
		}
	}
	else
	{
		LerpAlpha -= DeltaSeconds * LerpVelocity;
		if (LerpAlpha <= 0.f)
		{
			LerpAlpha = 0.f;
			bReverseRotation = false;
			bShouldRotate = false;
		}
	}

	// Interpolate between start and target rotation based on lerp alpha
	FRotator InterpolatedRotation = FMath::Lerp(StartRotation, TargetRotation, LerpAlpha);
	SetActorRotation(InterpolatedRotation);
}