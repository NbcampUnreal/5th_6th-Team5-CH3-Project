#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.generated.h"

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


};
