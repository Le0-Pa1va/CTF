// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingLevelSection.h"
#include "RotateSectionButton.generated.h"

UCLASS()
class PUZZLEGAME_API ARotateSectionButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateSectionButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "MainComponents")
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(EditInstanceOnly, Category = "LevelSectionInstance")
	ARotatingLevelSection* LevelSectionInstnace;

public:
	UFUNCTION()
	void PressButton();
};
