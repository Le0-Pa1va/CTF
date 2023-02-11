// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "InputActionValue.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "WallMergeActor.generated.h"


UCLASS()
class PUZZLEGAME_API AWallMergeActor : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

public:
	// Sets default values for this pawn's properties
	AWallMergeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category="MainComponents")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	UDecalComponent* CharacterDecal;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	UCameraComponent* CharCamera;

	UPROPERTY(EditAnywhere, Category="Movement")
	UPawnMovementComponent* PawnMovement;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

};
