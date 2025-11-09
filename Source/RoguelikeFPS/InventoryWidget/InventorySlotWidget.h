#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UItemBase;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UItemBase*, ClickedItem);

UCLASS()
class ROGUELIKEFPS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetItemData(UItemBase* InItem);

	UFUNCTION()
	void SetEmptySlot();

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;

	UPROPERTY()
	UItemBase* ItemData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* EmptySlotTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSlotClicked OnSlotClicked;

	UFUNCTION()
	void OnSlotClickedInternal();
};