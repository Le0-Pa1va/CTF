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

	FTransform CurrentTransform = GetActorTransform();
	FTransform LastValue = ActorTransforms.Last();

	if(bIsRecording == true && ActorTransforms.Num() > 0)
	{
		// CurrentTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
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
	if(GetVelocity().Size() <= 3.5f)
	{
		SetRecording(false);
		SetFoundFloor(true);
	}
	else
	{
		RecordPosition(CurrentActorTransform);
	}
}
