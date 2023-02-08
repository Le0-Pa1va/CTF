// Code by Leonardo Paiva 


#include "WallMergeActor.h"

// Sets default values
AWallMergeActor::AWallMergeActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CharacterDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	LineTracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	CharCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Remember to set the decal size on the blueprint (i think the  size 30, 30, 30)

	RootComponent = BoxCollision;
	CharacterDecal->SetupAttachment(RootComponent);
	LineTracePoint->SetupAttachment(CharacterDecal);
	CharCamera->SetupAttachment(CharacterDecal);

}

// Called when the game starts or when spawned
void AWallMergeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallMergeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWallMergeActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

