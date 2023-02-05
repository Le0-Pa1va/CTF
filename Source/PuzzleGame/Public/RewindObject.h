// Code by Leonardo Paiva 

#pragma once

#include "CoreMinimal.h"
#include "../PickUpObject.h"
#include "RewindObject.generated.h"

/**
 * 
 */

UCLASS()
class PUZZLEGAME_API ARewindObject : public APickUpObject
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
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<FVector> ActorLocations;

	UPROPERTY(VisibleInstanceOnly)
	TArray<FRotator> ActorRotations;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FTransform> ActorTransforms;

	//EditAnywhere for debug reasons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIsRewinding; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIsRecording;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bStartRewind; 

	UPROPERTY(VisibleAnywhere, Category = "Rewind")
	FTransform InitialPosition;

public:
	UFUNCTION()
	void RecordPosition(FTransform CurrentActorTransform);

	UFUNCTION()
	void Rewind(FTransform PositionLastIndex);
	
	UFUNCTION()
	FORCEINLINE void SetRecording(bool bStartRecord) { bIsRecording = bStartRecord; };

	UFUNCTION()
	FORCEINLINE void SetRewinding(bool bStartToRewind) { bStartRewind = bStartToRewind; };

	UFUNCTION()
	FORCEINLINE bool GetRewinding() { return bIsRewinding; };

};
