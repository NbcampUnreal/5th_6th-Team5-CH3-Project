#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameDataInstance.h"
//#include "Templates/ObjectPtr.h"  // 필수 현재 무기 데이터 없어서 생기는 문제
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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") FName AugmentID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") EAugmentCategory Category;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") EAugmentRarity Rarity;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") int32 HealthBonus;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") int32 AttackBonus;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") int32 DefenseBonus;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") int32 ShieldBonus;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") float AttackSpeedMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment") float MovingSpeedMultiplier;
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
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton1;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton2;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton3;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ConfirmButton;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentName1;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentName2;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentName3;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentDesc1;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentDesc2;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> AugmentDesc3;
private:
    UFUNCTION() void OnAugment1Clicked() { SelectAugment(0); }
    UFUNCTION() void OnAugment2Clicked() { SelectAugment(1); }
    UFUNCTION() void OnAugment3Clicked() { SelectAugment(2); }
    UFUNCTION() void OnConfirmClicked();
    void SelectAugment(int32 Index);
    void ApplyAugment(int32 Index);
    void UpdateConfirmButton();
    TWeakObjectPtr<AFPSCharacter> OwningCharacter;
    TArray<FAugmentData> AvailableAugments;
    int32 SelectedAugmentIndex = -1;
    UPROPERTY() TObjectPtr<UGameDataInstance> GameInstance;
};