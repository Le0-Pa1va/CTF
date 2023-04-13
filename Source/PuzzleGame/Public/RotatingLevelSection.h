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
	UPROPERTY(EditAnywhere, Category = "MainComponents")
	UStaticMeshComponent *SectionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SectionMovement")
	bool bShouldRotate;

	UPROPERTY(EditAnywhere, Category = "SectionMovement")
	FRotator StartRotation;

	UPROPERTY(EditAnywhere, Category = "SectionMovement")
	FRotator TargetRotation = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "SectionMovement")
	float LerpAlpha = 0.f;

	UPROPERTY(EditAnywhere, Category = "SectionMovement")
	float LerpVelocity = 10.f; // adjust this value to control the velocity of the lerp

	UPROPERTY(VisibleAnywhere, Category = "SectionMovement")
	bool bReverseRotation = false;

protected:
	UFUNCTION()
	void RotateSection(float DeltaSeconds);
	
public:
	UFUNCTION()
	FORCEINLINE bool GetShouldRotate() { return bShouldRotate; }
	
	UFUNCTION()
	FORCEINLINE void SetShouldRotate(bool bShouldRotateSection) { bShouldRotate = bShouldRotateSection; }
};
