

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LOL_Character.generated.h"

class UCameraComponent;
class UStaticMeshComponent;
class UBoxComponent;

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

protected:
	void MoveRight(float v);
	void Grapple();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
