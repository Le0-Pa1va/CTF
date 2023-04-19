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
		SetAttachmentToSection();
	}

	bShouldDetach = false;
}

// Called every frame
void ARewindObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetAttachParentActor() && bShouldDetach)
	{
		SetDettachmentToSection();
		SetSimulatePhysicsRewind(true);
	}
	else if(!GetAttachParentActor() && !bShouldDetach && LevelSectionInstance)
	{
		SetAttachmentToSection();
	}

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

	const FTransform LastValue = ActorTransforms.Last();
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
			SetDettachmentToSection();
			/* TODO:
			- Lerp the actor to the original position
			- Lerp the actor to the star position
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
			SetSimulatePhysicsRewind(false);
		}
	}
	else
	{
		SetActorTransform(InitialPosition);
		bIsRewinding = false;
		bStartRewind = false;
		SetSimulatePhysicsRewind(true);
		// this will have a parameter to tell if should enable gravity or not because the child class should not
		StaticMesh->SetEnableGravity(true);
	}
}

void ARewindObject::ReleasedObject(FTransform CurrentActorTransform)
{
	//needs a if here
	SetShouldDetach(false);
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

void ARewindObject::SetAttachmentToSection()
{
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	AttachToActor(LevelSectionInstance, AttachmentRules);
}

void ARewindObject::SetDettachmentToSection()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ARewindObject::SetSimulatePhysicsRewind(bool bShouldSimulate)
{
	StaticMesh->SetSimulatePhysics(bShouldSimulate);
}
