#include "LOL_MovingPlatform.h"
#include "Components/TimelineComponent.h"

// Sets default values
ALOL_MovingPlatform::ALOL_MovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	platformTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

	InterpFunction.BindUFunction(this, FName("ControlPlatform"));
	TimelineFinished.BindUFunction(this, FName("ReverseTimeline"));

	platformSpeed = 1.f;
}

// Called when the game starts or when spawned
void ALOL_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	isMoving = true;

	if (platformMovementCurve)
	{
		platformTimeline->AddInterpFloat(platformMovementCurve, InterpFunction, FName("Time"));
		platformTimeline->SetTimelineFinishedFunc(TimelineFinished);

		platformTimeline->SetLooping(false);

		platformTimeline->Play();
	}
}

void ALOL_MovingPlatform::ControlPlatform(float value)
{
	timelineValue = platformTimeline->GetPlaybackPosition();
	curveFloatValue = platformMovementCurve->GetFloatValue(timelineValue);

	FVector newPos = FMath::Lerp(startPos, endPos, value);

	MeshComp->SetRelativeLocation(newPos);
}

void ALOL_MovingPlatform::ReverseTimeline()
{
	if (platformTimeline->GetPlaybackPosition() == 0.f)
	{
		platformTimeline->Play();
	}
	else
	{
		platformTimeline->Reverse();
	}
	
}

// Called every frame
void ALOL_MovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}