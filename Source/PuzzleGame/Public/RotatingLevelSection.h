// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingLevelSection.generated.h"

UCLASS()
class PUZZLEGAME_API ARotatingLevelSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingLevelSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category="MainComponents")
	UStaticMeshComponent* SectionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SectionMovement")
	bool bShouldRotate;

	UPROPERTY(EditAnywhere, Category="SectionMovement")
	float SectionRotationRate = -90.f;
};
