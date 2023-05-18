// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../../PuzzleGameCharacter.h"
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

	UPROPERTY(VisibleAnywhere, Category="MainComponents")
	bool bCollidedCharacter;

	UPROPERTY(VisibleAnywhere, Category="Collision")
	bool bIsInFront;

	UPROPERTY(VisibleAnywhere, Category="Collision")
	bool bBackwardsCollision;

protected:
	UPROPERTY(EditAnywhere, Category="MainComponents")
	UBoxComponent* BoxCollision;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION()
	TArray<FVector> GetPolesLocations();

	UFUNCTION()
	FORCEINLINE bool GetCollidedCharater() { return bCollidedCharacter; };

	UFUNCTION()
	FORCEINLINE void SetCollidedCharater(bool bShouldRestoreCollision) { bCollidedCharacter = bShouldRestoreCollision; };

	UFUNCTION()
	FORCEINLINE bool GetIsInFront() { return bIsInFront; };

	UFUNCTION()
	FORCEINLINE bool GetBackwardsCollision() { return bBackwardsCollision; };

	UFUNCTION()
	FORCEINLINE void SetIsInFront(bool InFront) { bIsInFront = InFront; };
};
