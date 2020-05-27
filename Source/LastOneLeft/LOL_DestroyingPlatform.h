#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LOL_DestroyingPlatform.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class LASTONELEFT_API ALOL_DestroyingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALOL_DestroyingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
		UBoxComponent* BoxTrigger;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void HidePlatform();

	UFUNCTION()
		void ShowPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
		bool timerRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
		float maxTimeUntilDestroy;

private:
	FTimerHandle respawnTimer;
	FTimerHandle destroyTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};