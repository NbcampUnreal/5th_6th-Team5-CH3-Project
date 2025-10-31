// StatsComponent.cpp

#include "StatsComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h" 

UStatsComponent::UStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UStatsComponent::BeginPlay()
{
    Super::BeginPlay();

    // Health는 MaxHealth로 초기화하고 델리게이트를 발송합니다.
    CurrentHealth = MaxHealth;
    ClampHealth();
}

void UStatsComponent::ClampHealth()
{
    CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UStatsComponent::Heal(int32 HealAmount)
{
    if (HealAmount <= 0) return;
    CurrentHealth += HealAmount;
    ClampHealth();
}

float UStatsComponent::CalculateFinalDamage(float BaseDamage, bool& bWasCriticalHit)
{
    // BaseDamage는 무기의 순수 대미지 (헤드샷 배율 미적용 상태)

    bWasCriticalHit = (FMath::FRand() <= CritChance);

    // 1. 기본 공격력 적용
    float FinalDamage = BaseDamage * AttackDamage;

    // 2. 치명타 배율 적용
    if (bWasCriticalHit)
    {
        FinalDamage *= CritDamageMultiplier;
    }

    return FinalDamage;
}

float UStatsComponent::ApplyDamage(float DamageToApply)
{
    // 1. 방어력 계산
    float DamageTaken = FMath::Max(0.0f, DamageToApply - DefenseValue);

    // 2. 체력 감소
    CurrentHealth -= FMath::RoundToInt(DamageTaken);
    ClampHealth();

    return DamageTaken;
}

void UStatsComponent::AddXP(float Amount)
{
    if (CurrentLevel >= 99) return;

    CurrentXP += Amount;

#if WITH_EDITOR
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("XP Gained: %f"), Amount));
#endif

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
        XPToNextLevel = FMath::RoundToInt(XPToNextLevel * 1.5f);

        // 레벨업 시 스탯 보너스 적용 (예시)
        MaxHealth += 10;
        AttackDamage += 2.0f;
        DefenseValue += 1.0f;
        BaseMovementSpeed += 5.0f;

        bLeveledUp = true;
    }

    if (bLeveledUp)
    {
#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LEVEL UP! Level %d"), CurrentLevel));
#endif

        // 레벨업 시 체력 회복
        CurrentHealth = MaxHealth;
        ClampHealth();

        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
            if (PC)
            {
                OnLevelUp.Broadcast(PC);
            }
        }
    }
}

void UStatsComponent::ApplyAugment(int32 AugmentIndex)
{
    if (StatPointsAvailable <= 0) return;

    StatPointsAvailable--;

    switch (AugmentIndex)
    {
    case 1:
        MaxHealth += 20; Heal(20);
#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Augment 1 (Health) Applied."));
#endif
        break;
    case 2:
        AttackDamage += 5.0f;
#if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Augment 2 (Damage) Applied."));
#endif
        break;
    }
}