// Fill out your copyright notice in the Description page of Project Settings.


#include "RewindObject.h"

// Sets default values
ARewindObject::ARewindObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void ARewindObject::BeginPlay()
{
	Super::BeginPlay();
	
	bIsRewinding = false;
	ActorTransforms.Add(GetActorTransform());
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
		CurrentLocation != LastValue.GetLocation() &&
		CurrentRotation != LastValue.GetRotation().Rotator()
	)
	{
		ActorTransforms.Add(CurrentTransform);
	}

	if(bIsRewinding == true && ActorTransforms.Num() > 1)
	{
		SetActorTransform(LastValue);
		ActorTransforms.Pop();
	}
}
