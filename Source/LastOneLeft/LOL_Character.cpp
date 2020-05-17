#include "LOL_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "CableComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALOL_Character::ALOL_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	CableComp = CreateDefaultSubobject<UCableComponent>(TEXT("CableComp"));
	CableComp->SetupAttachment(MeshComp);
	CableComp->SetVisibility(false, true);
}

void ALOL_Character::MoveRight(float v)
{
	if ((Controller) && (v != 0.0f))
	{
		AddMovementInput(GetActorRightVector(), v);
	}
}

void ALOL_Character::Grapple()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	FVector playerLoc = playerController->GetPawn()->GetActorLocation();

	//find where the player clicks
	FHitResult underCursorHitResult;
	playerController->GetHitResultUnderCursor(ECC_Visibility, true, underCursorHitResult);

	//rotate our temp object toward clicked location
	MeshComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(playerLoc, underCursorHitResult.Location));

	//calculate the end location of the line trace based on a set length
	FVector endLoc = (MeshComp->GetForwardVector() * 500) + playerLoc;	

	//do the line trace for anything blocking visibility
	FHitResult traceHitResult;
	GetWorld()->LineTraceSingleByChannel(traceHitResult, playerLoc, endLoc, ECC_Visibility);

	//if we hit an actor
	if (traceHitResult.GetActor() != nullptr)
	{
		//if hit actor is a platform
		if (traceHitResult.Component->ComponentHasTag(FName("Platform")))
		{
			//debug grapple location
			DrawDebugLine(GetWorld(), playerLoc, traceHitResult.Actor->GetActorLocation(), FColor::Green, true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("actor is platform"));
			
			//attach cable to hit result location
			CableComp->SetVisibility(true, true);
			CableComp->SetAttachEndToComponent(traceHitResult.GetComponent());
			CableComp->EndLocation = traceHitResult.Location - traceHitResult.GetComponent()->GetComponentLocation();
			CableComp->EndLocation.X = traceHitResult.GetComponent()->GetComponentLocation().X;

			//set grapple location (for GrappleMovement function to handle)
			grappleToLoc = traceHitResult.Location;
			
		}

	}
	
}

void ALOL_Character::GrappleMovement()
{

}

void ALOL_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ALOL_Character::Grapple);
	

	PlayerInputComponent->BindAxis("MoveRight", this, &ALOL_Character::MoveRight);
}

void ALOL_Character::Tick(float DeltaTime)
{
	//if ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - grappleToLoc).Size() > 100)
	//	GrappleMovement();
}

