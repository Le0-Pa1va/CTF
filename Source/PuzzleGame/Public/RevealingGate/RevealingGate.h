// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RevealingGate.generated.h"

UCLASS()
class PUZZLEGAME_API ARevealingGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARevealingGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category="MainComponents")
	UStaticMeshComponent* FirstPole;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	UStaticMeshComponent* SecondPole;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	USceneComponent* GateRoot;

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPolesLocations();
};
