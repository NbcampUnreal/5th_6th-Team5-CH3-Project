#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

UCLASS()
class ROGUELIKEFPS_API UItemBase : public UObject
{
    GENERATED_BODY()

public:

    // 공통 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EPartGrade PartGrade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    EPartType PartType = EPartType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Thumbnail;

    // 능력치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    float BaseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    float BaseAttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    int32 Ammo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    int32 Amount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    int32 BuyPrice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    int32 SellPrice;

public:
    void InitItemData(const FItemData& Data);

    void ApplyToWeapon(float& OutDamage, int32& OutAmmo, float& OutAttackSpeed) const;
};