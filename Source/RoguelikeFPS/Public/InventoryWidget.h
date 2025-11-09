#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventory;
class UUniformGridPanel; 
class UTextBlock;
class UInventorySlotWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemClicked, UItemBase*, ClickedItem);

UCLASS()
class ROGUELIKEFPS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitInventory(UInventory* InInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateUI();

	UFUNCTION()
	void UpdateGoldUI(int32 NewGold);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInventoryItemClicked OnItemClicked;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	UInventory* Inventory;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemGrid;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Columns = 10;

	UFUNCTION()
	void HandleSlotClicked(UItemBase* ClickedItem);
};