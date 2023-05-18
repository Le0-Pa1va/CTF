// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RevealingGate.h"
#include "RevealingObject.generated.h"

UCLASS()
class PUZZLEGAME_API ARevealingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARevealingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainComponents")
	UStaticMeshComponent* ObjectMesh;

protected:
	UPROPERTY(EditAnywhere, Category="Poles")
	ARevealingGate* RevealingGate;

	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaskingMaterialInstance;

	UPROPERTY(VisibleAnywhere, Category="Materials")
	UMaterialInstanceDynamic* DynMaterial;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainComponents")
	float ShouldStartShowingScalar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainComponents")
	float OpacityAfterCollision;

private:
	UPROPERTY(VisibleAnywhere, Category="Collision")
	ACharacter* MainCharacter;

	UPROPERTY(VisibleAnywhere, Category="Collision")
	bool bCollidedGate;

private:
	UFUNCTION()
	void SetCollision(float Opacity);

	UFUNCTION()
	void SetDynMaterialParam(FName ParamName, float ParamValue);
};
