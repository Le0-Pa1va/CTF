// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InterpToMovementComponent.h"
#include "FloatingActor.generated.h"

UCLASS()
class PUZZLEGAME_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	UPROPERTY(EditAnywhere, Category="MainComponents")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Movement")
	UInterpToMovementComponent* InterpComponent;
};
