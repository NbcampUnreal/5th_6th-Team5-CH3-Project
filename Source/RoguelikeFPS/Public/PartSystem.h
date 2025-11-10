#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "PartSystem.generated.h"

class UItemBase;
class UInventory;
class AFPSCharacter;

USTRUCT(BlueprintType)
struct FEquippedParts
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* Muzzle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* Magazin = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* Stock = nullptr;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UPartSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UPartSystem();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void EquipPart(UItemBase* Item, UInventory* Inventory);

    UFUNCTION(BlueprintCallable)
    void UnequipPart(EPartType PartType, UInventory* Inventory);

    UFUNCTION(BlueprintCallable)
    void RecalculateStats();

    float DefaultDamage;
    float DefaultAttackSpeed;
    int32 DefaultAmmo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Part")
    FEquippedParts EquippedParts;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Result Stats")
    float FinalDamage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Result Stats")
    float FinalAttackSpeed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Result Stats")
    int32 FinalAmmo;

    UPROPERTY()
    UInventory* LinkedInventory;

    UPROPERTY()
    AFPSCharacter* OwnerCharacter;

    UPROPERTY()
    UItemBase* PendingEquipItem = nullptr;

    UFUNCTION(BlueprintCallable)
    void SetPendingPart(UItemBase* Item);

protected:

    void ApplyEquippedParts();
};
