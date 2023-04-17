// Code by Leonardo Paiva 


#include "RewindObject.h"

// Sets default values
ARewindObject::ARewindObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LevelSectionInstance = CreateDefaultSubobject<ARotatingLevelSection>(TEXT("LevelSectionInstance"));
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

	if (LevelSectionInstance)
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
		
		AttachToActor(LevelSectionInstance, AttachmentRules);
	}
}

// Called every frame
void ARewindObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(LevelSectionInstance)
	{
		FTransform LevelSectionTransform = LevelSectionInstance->GetTransform();
		FTransform RewindActorTransform = GetTransform();
		CurrentTransform = RewindActorTransform.GetRelativeTransform(LevelSectionTransform);
	}
	else
	{
		CurrentTransform = GetActorTransform();
	}
	FTransform LastValue = ActorTransforms.Last();

	if(bIsRecording == true && ActorTransforms.Num() > 0)
	{
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
	if (ActorTransforms.Num() > 1)
	{
		bIsRewinding = true;
		if (LevelSectionInstance)
		{
			/* TODO:
			- Lerp the actor to the original position
			- Lerp the actor to the star position
			- fix actor falling of the ground
			- test using just the location instead of transform
			*/
			FTransform SectionWorldTransform = LevelSectionInstance->GetActorTransform();
			// Combine the section's world transform with the relative transform
			FTransform WorldTransform = PositionLastIndex * SectionWorldTransform;
			SetActorRelativeTransform(WorldTransform);
		}
		else
		{
			SetActorTransform(PositionLastIndex);
		}
		ActorTransforms.Pop();
		if (StaticMesh->IsSimulatingPhysics())
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
	//90.f is the tolerance
	if(GetVelocity().Equals(FVector(0.f, 0.f, 0.f), 90.f))
	{
		SetRecording(false);
		SetFoundFloor(true);
	}
	else
	{
		RecordPosition(CurrentActorTransform);
	}
}
