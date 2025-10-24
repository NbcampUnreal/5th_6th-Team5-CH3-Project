#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKEFPS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Inventory")
	TArray<UItemBase*> Items;
	UFUNCTION(BlueprintCallable)
	void AddItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemBase* Item);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	int32 Gold;
	UFUNCTION(BlueprintCallable)
	bool BuyItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable)
	bool SellItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	void ShowInventory() const;
	//임시
	UFUNCTION(BlueprintCallable)
	void TestInventory();

	//검색
	UFUNCTION(BlueprintCallable)
	UItemBase* SearchItemName(const FName& Name) const;
	UFUNCTION(BlueprintCallable)
	UItemBase* SearchItemNumber(int32 ItemNumber) const;
	
};
