#include "TeleportVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "Engine/World.h"
#include "GameDataInstance.h"
#include "FPSGameMode.h"
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

    UGameDataInstance* GameData = Cast<UGameDataInstance>(GetGameInstance());
    if (!GameData) return;

    GameData->IncrementTeleportCount();

    if (GameData->ShouldTriggerGameClear())
    {
        if (AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            GameMode->SetGameState(EGameState::GameClear);
        }
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("레벨로 텔레포트: %s"), *NextLevelName.ToString()));
    }

    UWorld* World = GetWorld();
    if (!World) return;

    GameData->CurrentStageIndex++;
    if (GameData->StageLevelNames.IsValidIndex(GameData->CurrentStageIndex))
    {
        UGameplayStatics::OpenLevel(World, GameData->StageLevelNames[GameData->CurrentStageIndex]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("더 이상 스테이지가 없습니다!"));
        if (AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            GameMode->SetGameState(EGameState::GameClear);
        }
    }
}