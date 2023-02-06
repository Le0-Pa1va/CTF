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
	
	bIsRecording = false;
	bStartRewind = false;
	InitialPosition = GetActorTransform();
	ActorTransforms.Add(InitialPosition);
	SetFoundFloor(true);
}

// Called every frame
void ARewindObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO this only needs to be a FVector that gets the location instead of FTransform, so the object will never rotates
	FTransform CurrentTransform = GetActorTransform();
	FTransform LastValue = ActorTransforms.Last();

	if(bIsRecording == true && ActorTransforms.Num() > 0)
	{
		CurrentTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
		RecordPosition(CurrentTransform);
	}

	if(bIsRecording == false && bStartRewind == true)
	{
		Rewind(LastValue);
	}

	if(bFoundFloor == false)
	{
		ReleasedObject(CurrentTransform);
	}
}

void ARewindObject::RecordPosition(FTransform CurrentActorTransform)
{
	ActorTransforms.Add(CurrentActorTransform);
}

void ARewindObject::Rewind(FTransform PositionLastIndex)
{
	if(ActorTransforms.Num() > 1)
		{
			bIsRewinding = true;
			SetActorTransform(PositionLastIndex);
			ActorTransforms.Pop();
			if(StaticMesh->IsSimulatingPhysics())
			{
				StaticMesh->SetEnableGravity(false);
				StaticMesh->SetSimulatePhysics(false);
			}
		}
	else
		{
			SetActorTransform(InitialPosition);
			bIsRewinding = false;
			bStartRewind = false;
			StaticMesh->SetSimulatePhysics(true);
			StaticMesh->SetEnableGravity(true);
		}
}
void ARewindObject::ReleasedObject(FTransform CurrentActorTransform)
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector DownwardsVector = FVector(0.f, 0.f, -1.f);
	FVector TraceEnd = GetActorLocation() + DownwardsVector * TraceLenght;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if(Hit.Distance < 5 && GetVelocity() == FVector(0.f, 0.f, 0.f))
	{
		SetRecording(false);
		SetFoundFloor(true);
	}
	else
	{
		RecordPosition(CurrentActorTransform);
	}
}
