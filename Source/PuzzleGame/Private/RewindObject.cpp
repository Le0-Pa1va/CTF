// Code by Leonardo Paiva 


#include "RewindObject.h"

// Sets default values
ARewindObject::ARewindObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARewindObject::BeginPlay()
{
	Super::BeginPlay();
	
	bIsRewinding = false;
	InitialPosition = GetActorTransform();
	ActorTransforms.Add(InitialPosition);
}

// Called every frame
void ARewindObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	FTransform CurrentTransform = GetActorTransform();
	FTransform LastValue = ActorTransforms.Last();
	if(
		bIsRewinding == false &&
		ActorTransforms.Num() > 0 &&
		(CurrentLocation != LastValue.GetLocation() ||
		CurrentRotation != LastValue.GetRotation().Rotator())
	)
	{
		ActorTransforms.Add(CurrentTransform);
	}

	if(bIsRewinding == true)
	{
		if(ActorTransforms.Num() > 1)
		{
			SetActorTransform(LastValue);
			ActorTransforms.Pop();
		}
		else
		{
			SetActorTransform(InitialPosition);
		}
	}
}
