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
	PrimaryActorTick.bCanEverTick = true;

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
void ALOL_Character::Blast()
{
	if (canBlast)
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		FVector playerLoc = GetPlayerLoc(playerController);
		FVector mouseLoc = GetMouseLoc(playerController);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("blasting"));
		mouseLoc.X = 0;

		LaunchCharacter((mouseLoc - playerLoc) * 4, false, false);
		canBlast = false;
	}
}

FVector ALOL_Character::GetPlayerLoc(APlayerController* playerController)
{
	return playerController->GetPawn()->GetActorLocation();
}

FVector ALOL_Character::GetMouseLoc(APlayerController* playerController)
{
	FHitResult underCursorHitResult;
	playerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, underCursorHitResult);
	return underCursorHitResult.Location;
}

void ALOL_Character::StopGrapple()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("released mouse button"));
	grappleStop = true;
	if (shouldGrapple)
	{
		float intensity = 400.0f;
		float dist = (grappleToLoc - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()).Size();

		LaunchCharacter(FVector(0, 0, intensity), false, false);
	}
}

void ALOL_Character::Grapple()
{
	grappleStop = false;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	//FVector playerLoc = playerController->GetPawn()->GetActorLocation();
	FVector playerLoc = GetPlayerLoc(playerController);

	//find where the player clicks
	FVector mouseLoc = GetMouseLoc(playerController);
	

	//rotate our temp object toward clicked location
	MeshComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(playerLoc, mouseLoc));

	//calculate the end location of the line trace based on a set length
	FVector endLoc = (MeshComp->GetForwardVector() * 1000) + playerLoc;

	//do the line trace for anything blocking visibility
	FHitResult traceHitResult;
	GetWorld()->LineTraceSingleByChannel(traceHitResult, playerLoc, endLoc, ECC_Visibility);
	//DrawDebugLine(GetWorld(), playerLoc, endLoc, FColor::Green, true);

	//if we hit an actor
	if (traceHitResult.GetActor() != nullptr)
	{
		//if hit actor is a platform
		if (traceHitResult.Component->ComponentHasTag(FName("Platform")))
		{
			//debug grapple location
			//DrawDebugLine(GetWorld(), playerLoc, traceHitResult.Actor->GetActorLocation(), FColor::Green, true);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("actor is platform"));
			
			//attach cable to hit result location
			CableComp->SetVisibility(true, true);
			CableComp->SetAttachEndToComponent(traceHitResult.GetComponent());
			CableComp->EndLocation = traceHitResult.Location - traceHitResult.GetComponent()->GetComponentLocation();
			CableComp->EndLocation.X = traceHitResult.GetComponent()->GetComponentLocation().X;

			//set grapple location (for GrappleMovement function to handle)
			grappleToLoc = traceHitResult.Actor->GetActorLocation();
			
			shouldGrapple = true;
		}
		else
		{
			shouldGrapple = false;
		}

	}
	
}

void ALOL_Character::GrappleMovement()
{
	//if ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - grappleToLoc).Size() > 180 && grappleStop == false)
	if (grappleStop == false)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("grappling to point"));
		LaunchCharacter((grappleToLoc - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) * 0.1, false, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, grappleToLoc.ToString());
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("DONE grapple"));
		shouldGrapple = false;
		CableComp->SetVisibility(false, true);
		grappleStop = false;
	}
}

void ALOL_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ALOL_Character::Grapple);
	PlayerInputComponent->BindAction("Grapple", IE_Released, this, &ALOL_Character::StopGrapple);
	PlayerInputComponent->BindAction("Blast", IE_Pressed, this, &ALOL_Character::Blast);
	

	PlayerInputComponent->BindAxis("MoveRight", this, &ALOL_Character::MoveRight);
}

void ALOL_Character::Tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString(canBlast ? TEXT("true") : TEXT("false")));
	if (shouldGrapple)
	{
		GrappleMovement();
	}
	if (CanJump() && !canBlast)
	{
		canBlast = true;
	}
	
}

