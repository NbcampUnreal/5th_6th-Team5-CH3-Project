#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "AugmentSystem.generated.h"

UENUM(BlueprintType)
enum class EAugmentRarity : uint8
{
    Common,
    Rare,
    Epic
};

USTRUCT(BlueprintType)
struct FAugmentData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AugmentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAugmentRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealthBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageBonus;

    // 추가 속성 (필요 시 확장)
};

UCLASS()
class ROGUELIKEFPS_API UAugmentSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UPROPERTY(EditDefaultsOnly, Category = "Augment")
    UDataTable* AugmentDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Augment")
    TMap<EAugmentRarity, float> RarityWeights;

    UFUNCTION(BlueprintCallable, Category = "Augment")
    TArray<FAugmentData> GetRandomAugments(int32 Count = 3);

    UFUNCTION(BlueprintCallable, Category = "Augment")
    void ApplyAugment(class AFPSCharacter* Character, FName AugmentID);
};