#include "TeleportVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameDataInstance.h" // GameDataInstance 포함
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
            Character->GainGold(50);
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

    UWorld* World = GetWorld();
    if (!World) return;

    FName LevelToOpen = LevelAfterLastStage; // 기본값은 액터 속성의 LevelAfterLastStage 사용
    bool bIsFinalClear = false;

    // GameDataInstance에서 현재 스테이지 확인
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);
    if (UGameDataInstance* GameDataInstance = Cast<UGameDataInstance>(GameInstance))
    {
        // CurrentStageIndex가 StageLevelNames의 개수보다 크면 (즉, 마지막 맵을 통과했다면) 클리어
        if (GameDataInstance->CurrentStageIndex > GameDataInstance->StageLevelNames.Num())
        {
            bIsFinalClear = true;
            LevelToOpen = GameDataInstance->MainMenuLevelName; // 메인 메뉴 레벨로 이동
        }
        else
        {
            // 일반적인 맵 이동 (이 텔레포트가 일반 맵 이동용으로 사용될 경우)
            // 주의: 이 텔레포트를 맵 1 -> 맵 2 이동용으로 쓴다면 LevelAfterLastStage 대신 다음 맵 이름을 지정해야 함.
            // 현재는 '클리어 지점' 역할에 초점을 맞춰 GameDataInstance의 인덱스를 기준으로 판단합니다.
            
            // StageLevelNames 배열에 접근할 때는 CurrentStageIndex - 1 사용 (GameMode에서 이미 증가시켰을 가정)
            if (GameDataInstance->CurrentStageIndex >= 1 && 
                GameDataInstance->CurrentStageIndex <= GameDataInstance->StageLevelNames.Num())
            {
                LevelToOpen = GameDataInstance->StageLevelNames[GameDataInstance->CurrentStageIndex - 1];
                // StageIndex는 이미 GameMode에서 증가되었으므로, 여기서 추가 증가는 안 함
            }
        }
    }

    if (GEngine)
    {
        if (bIsFinalClear)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("GAME CLEARED! Returning to Main Menu."));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Teleporting to level: %s"), *LevelToOpen.ToString()));
        }
    }

    UGameplayStatics::OpenLevel(World, LevelToOpen);
}