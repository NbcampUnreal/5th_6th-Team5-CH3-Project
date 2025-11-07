#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventory;
class UScrollBox;
class UTextBlock;
class UInventoryItemWidget;

UCLASS()
class ROGUELIKEFPS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitInventory(UInventory* InInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateUI();
	
protected:

	UPROPERTY(BlueprintReadOnly)
	UInventory* Inventory;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
};