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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainComponents")
	UDecalComponent* CharacterDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainComponents")
	UCameraComponent* CharCamera;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	USceneComponent* ActorCenter;

	UPROPERTY(EditAnywhere, Category="MainComponents")
	USceneComponent* PivotPoint;

	UPROPERTY(EditAnywhere, Category="Movement")
	UPawnMovementComponent* PawnMovement;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TraceLenght = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SidewayTraceLenght = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DistanceFromCenter = 200.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bHitOnFront;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bHitOnSideways;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float YawOnCorner;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsRotatingConcave;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsRotatingConvex;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float InitialDistance;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float CurrentDistance;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	float GetForwardImpactAngle(float XMovementValue);

	UFUNCTION()
	void GetSidewaysImpact(float XMovementValue);

	UFUNCTION()
	void SetPivotPoint(USceneComponent* NewPivotPoint);

};
