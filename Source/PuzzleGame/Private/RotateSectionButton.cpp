// Code by Leonardo Paiva 


#include "RotateSectionButton.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARotateSectionButton::ARotateSectionButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	PressButtonSound = CreateDefaultSubobject<USoundBase>(TEXT("PressButtonSound"));

}

// Called when the game starts or when spawned
void ARotateSectionButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotateSectionButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARotateSectionButton::PressButton()
{
	if(PressButtonSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PressButtonSound,  GetActorLocation());
	}
	
	if(LevelSectionInstnace && !LevelSectionInstnace->GetShouldRotate())
	{
		LevelSectionInstnace->SetShouldRotate(true);
	}
}

