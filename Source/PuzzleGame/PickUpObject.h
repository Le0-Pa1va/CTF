// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpObject.generated.h"

UCLASS()
class PUZZLEGAME_API APickUpObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//Object Mesh
	UPROPERTY(EditAnywhere, Category = "Object Properties")
	UStaticMeshComponent* StaticMesh;

};
