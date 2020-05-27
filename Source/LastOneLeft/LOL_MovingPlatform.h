#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "LOL_MovingPlatform.generated.h"

class UStaticMeshComponent;

UCLASS()
class LASTONELEFT_API ALOL_MovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALOL_MovingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool isMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		float platformSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		UCurveFloat* platformMovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		FVector startPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		FVector endPos;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void ControlPlatform(float value);

	UFUNCTION()
		void ReverseTimeline();

	UPROPERTY()
		UTimelineComponent* platformTimeline;

	UPROPERTY()
		FOnTimelineFloat InterpFunction {};

	UPROPERTY()
		FOnTimelineEvent TimelineFinished {};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Platform")
		float timelineValue;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Platform")
		float curveFloatValue;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};