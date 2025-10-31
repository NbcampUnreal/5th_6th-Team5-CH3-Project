#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Inventory")
	TArray<UItemBase*> InventoryItems;

	UFUNCTION(BlueprintCallable)
	void AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemBase* Item, int32 Amount = 1);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	int32 Gold;

	UFUNCTION(BlueprintCallable)
	bool BuyItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	bool SellItem(UItemBase* Item, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	void ShowInventory() const;

	//°Ë»ö
	UFUNCTION(BlueprintCallable)
	UItemBase* SearchItemName(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	UItemBase* SearchItemNumber(int32 ItemNumber) const;
	
};
