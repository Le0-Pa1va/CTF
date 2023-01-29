// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "RewindObject.generated.h"

UCLASS()
class PUZZLEGAME_API ARewindObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARewindObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StaticMesh")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<FVector> ActorLocations;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FRotator> ActorRotations;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FTransform> ActorTransforms;

	//EditAnywhere for debug reasons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIsRewinding; 

};
