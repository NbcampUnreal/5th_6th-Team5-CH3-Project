#include "TeleportVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ATeleportVolume::ATeleportVolume()
{
    TeleportTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportTrigger"));
    RootComponent = TeleportTrigger;

    TeleportTrigger->SetCollisionProfileName(TEXT("Trigger"));
    TeleportTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATeleportVolume::OnOverlapBegin);
}

void ATeleportVolume::BeginPlay()
{
    Super::BeginPlay();
}

void ATeleportVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    if (OtherActor->IsA<AFPSCharacter>())
    {
        if (AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("LEVELEXP"));
            Character->AddXP(50.0f);
            //Character->GainGold(50);
            //Character->TakeDamage(50.0f, , , this);
        }
        // 기존 타이머가 있으면 클리어
        if (GetWorldTimerManager().IsTimerActive(TeleportTimerHandle))
        {
            GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
        }

        PendingTeleportActor = OtherActor;

        // 타이머 등록 ? TeleportPendingActor 를 호출
        GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ATeleportVolume::TeleportPendingActor, DelayBeforeTeleport, false);

        // (선택) 디버그 로그/카운트다운 UI 트리거 가능
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Teleport countdown started"));
        }
    }
}

void ATeleportVolume::TeleportPendingActor()
{
    AActor* ActorToTeleport = PendingTeleportActor.Get();
    if (!ActorToTeleport) return;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Teleporting to level: %s"), *NextLevelName.ToString()));
    }

    // 컨트롤러가 있다면 그 컨텍스트로 열고 아니라면 World로 열기
    UWorld* World = GetWorld();
    if (!World) return;

    UGameplayStatics::OpenLevel(World, NextLevelName);
}
