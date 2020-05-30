#include "LOL_DestroyingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALOL_DestroyingPlatform::ALOL_DestroyingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = MeshComp;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(MeshComp);
	BoxTrigger->SetCollisionProfileName(TEXT("Trigger"));
	BoxTrigger->SetBoxExtent(FVector(90.f, 113.f, 30.f));
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALOL_DestroyingPlatform::OnOverlapBegin);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ALOL_DestroyingPlatform::OnOverlapEnd);
}

void ALOL_DestroyingPlatform::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorldTimerManager().SetTimer(destroyTimer, this, &ALOL_DestroyingPlatform::HidePlatform, 5.f, false);
}

void ALOL_DestroyingPlatform::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HidePlatform();
	GetWorldTimerManager().SetTimer(respawnTimer, this, &ALOL_DestroyingPlatform::ShowPlatform, 5.f, false);
}

void ALOL_DestroyingPlatform::HidePlatform()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ALOL_DestroyingPlatform::ShowPlatform()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ALOL_DestroyingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALOL_DestroyingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}