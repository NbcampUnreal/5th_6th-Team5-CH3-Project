#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacter.h"
#include "AugmentWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UAugmentWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SelectAugment(FName AugmentName);

protected:
    // 캐릭터 포인터는 UPROPERTY로 GC 안전성 확보
    UPROPERTY()
    AFPSCharacter* PlayerCharacter = nullptr;
};

// 증강 등급 Enum
//UENUM(BlueprintType)
//enum class EAugmentRarity : uint8
//{
//    Normal UMETA(DisplayName = "Normal"),
//    Epic UMETA(DisplayName = "Epic"),
//    Legendary UMETA(DisplayName = "Legendary")
//};
//
//// 증강 범주 Enum (이전에 정의한 것)
//UENUM(BlueprintType)
//enum class EAugmentCategory : uint8
//{
//    Damage UMETA(DisplayName = "Damage"),
//    Utility UMETA(DisplayName = "Utility"),
//    Defense UMETA(DisplayName = "Defense"),
//    None UMETA(DisplayName = "None")
//};
//
//// 증강 데이터를 담을 구조체 (FTableRowBase를 상속받아 데이터 테이블에서 사용 가능)
//USTRUCT(BlueprintType)
//struct FAugmentData : public FTableRowBase
//{
//    GENERATED_BODY()
//
//    // 증강의 고유 이름 (데이터 테이블의 Row Name과 동일하게 사용)
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
//    FName AugmentID;
//
//    // UI에 표시될 이름
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
//    FText DisplayName;
//    
//    // UI에 표시될 설명
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
//    FText Description;
//    
//    // 증강 범주
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
//    EAugmentCategory Category;
//    
//    // 증강 등급
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Augment")
//    EAugmentRarity Rarity;
//};