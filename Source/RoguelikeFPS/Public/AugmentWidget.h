#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "AugmentWidget.generated.h"

// 증강 희귀도 Enum
UENUM(BlueprintType)
enum class EAugmentRarity : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),
    Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EAugmentCategory : uint8
{
    Damage UMETA(DisplayName = "Damage"),
    Utility UMETA(DisplayName = "Utility"),
    Defense UMETA(DisplayName = "Defense"),
    None UMETA(DisplayName = "None")
};

// 증강 데이터 구조체
USTRUCT(BlueprintType)
struct FAugmentData : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    FName AugmentID; // 증강 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    FText DisplayName; // UI에 표시될 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    FText Description; // UI에 표시될 설명
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    EAugmentCategory Category; // 증강 범주
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    EAugmentRarity Rarity; // 증강 희귀도
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    int32 HealthBonus; // 체력 보너스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    int32 AttackBonus; // 공격력 보너스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    int32 DefenseBonus; // 방어력 보너스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    int32 ShieldBonus; // 쉴드 보너스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    float AttackSpeedMultiplier; // 공격 속도 배수 (예: 1.1 = 10% 증가)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
    float MovingSpeedMultiplier; // 이동 속도 배수 (예: 1.2 = 20% 증가)
};

UCLASS()
class ROGUELIKEFPS_API UAugmentWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual bool Initialize() override;
    UFUNCTION(BlueprintCallable, Category = "Augment")
    void Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AugmentButton1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AugmentButton2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AugmentButton3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentName1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentName2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentName3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentDesc1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentDesc2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AugmentDesc3;

private:
    UFUNCTION()
    void OnAugment1Clicked();
    UFUNCTION()
    void OnAugment2Clicked();
    UFUNCTION()
    void OnAugment3Clicked();
    void ApplyAugment(int32 AugmentIndex);
    TWeakObjectPtr<AFPSCharacter> OwningCharacter;
    TArray<FAugmentData> AvailableAugments;
};