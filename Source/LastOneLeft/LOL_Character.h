#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LOL_Character.generated.h"

class UCameraComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UCableComponent;
class USoundCue;
class UAudioComponent;

UCLASS()
class LASTONELEFT_API ALOL_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALOL_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collider")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cable")
	UCableComponent* CableComp;

protected:
	// Functions:
	void MoveRight(float v);
	void Grapple();
	void GrappleMovement();
	void StopGrapple();
	void Blast();
	FVector GetPlayerLoc(APlayerController* playerController);
	FVector GetMouseLoc(APlayerController* playerController);
	AActor* platform;

	// Variables:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
		FVector grappleOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
		FVector grappleEndLoc;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
		bool shouldGrapple = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple")
		bool grappleStop = false;
	UPROPERTY(EditAnywhere, Category = "Grapple")
		float grappleDist = 1000;
	UPROPERTY(EditAnywhere, Category = "Grapple")
		float grappleSpeed = 0.05;
	UPROPERTY(EditAnywhere, Category = "Grapple")
		float grappleJumpIntensity = 400.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blast")
		bool canBlast = true;
	UPROPERTY(EditAnywhere, Category = "Blast")
		float blastIntensity = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Walk")
		FVector playerVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		bool isJump = true;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue* grappleImpactAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue* grappleReleaseAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue* grappleThrowAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue* jumpSpellAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		UAudioComponent* grappleAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		UAudioComponent* jumpAudioComponent;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Test")
		void animateBlast();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};