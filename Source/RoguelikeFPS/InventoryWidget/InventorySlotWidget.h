#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UItemBase;
class UImage;

UCLASS()
class ROGUELIKEFPS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetItemData(UItemBase* InItem);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY()
	UItemBase* ItemData;
};