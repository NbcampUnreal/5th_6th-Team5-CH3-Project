#include "StatsComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UStatsComponent::UStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    // 초기 XP 설정 로직 (필요시)
}

void UStatsComponent::AddXP(float Amount)
{
    if (CurrentLevel >= 99) return; // 최대 레벨 제한

    CurrentXP += Amount;

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("XP Gained: %f"), Amount));

    TryLevelUp();
}

void UStatsComponent::TryLevelUp()
{
    bool bLeveledUp = false;

    while (CurrentXP >= XPToNextLevel)
    {
        CurrentXP -= XPToNextLevel;
        CurrentLevel++;
        StatPointsAvailable++;
        XPToNextLevel *= 1.5f; // 다음 레벨 필요 XP 증가
        bLeveledUp = true;
    }

    if (bLeveledUp)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LEVEL UP! Level %d"), CurrentLevel));

        // 소유 액터의 컨트롤러를 가져옴
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
            if (PC)
            {
                // 레벨업 이벤트를 발송하여 GameMode에게 알립니다.
                OnLevelUp.Broadcast(PC);
            }
        }
    }
}

void UStatsComponent::ApplyAugment(int32 AugmentIndex)
{
    if (StatPointsAvailable <= 0) return;

    StatPointsAvailable--;

    // 증강 적용 로직 (스텟 변경, 특수 능력 부여 등)
    switch (AugmentIndex)
    {
    case 1:
        // 예: 체력 증가 로직
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Augment 1 (Health) Applied."));
        break;
    case 2:
        // 예: 이동 속도 증가 로직
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Augment 2 (Speed) Applied."));
        break;
        // ...
    }
}
