// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(EditDefaultsOnly, Category="Static Mesh")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<FVector> ActorLocation;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FRotator> ActorRotation;

	UPROPERTY(VisibleAnywhere)
	bool bIsRewinding; 

};
