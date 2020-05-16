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

	FHitResult underCursorHitResult;

	playerController->GetHitResultUnderCursor(ECC_Visibility, true, underCursorHitResult);

	MeshComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(playerLoc, underCursorHitResult.Location));

	FVector endLoc = (MeshComp->GetForwardVector() * 500) + playerLoc;	

	FHitResult traceHitResult;

	GetWorld()->LineTraceSingleByChannel(traceHitResult, playerLoc, endLoc, ECC_Visibility);

	//DrawDebugLine(GetWorld(), playerLoc, endLoc, FColor::Red, true);

	//attach cable to hit result location

	if (traceHitResult.GetActor() != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(traceHitResult.GetActor()->GetName()));
		DrawDebugLine(GetWorld(), playerLoc, traceHitResult.Actor->GetActorLocation(), FColor::Green, true);
		
		if (traceHitResult.Component->ComponentHasTag(FName("Platform")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("actor has tag"));



			GetWorld()->SpawnActor<ACableActor>(ACableActor::StaticClass(), playerLoc);

		}

	}
	else
	{
		DrawDebugLine(GetWorld(), playerLoc, endLoc, FColor::Red, true);
	}

	

	//apply force to player
}

void ALOL_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ALOL_Character::Grapple);
	

	PlayerInputComponent->BindAxis("MoveRight", this, &ALOL_Character::MoveRight);
}

