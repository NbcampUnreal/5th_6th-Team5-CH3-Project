#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMenuWidget.generated.h"

class UCraftingWidget;
class UInventoryWidget;
class UCraftingSystem;
class UInventory;
class UButton;
class UTextBlock;
class UDataTable;

UCLASS()
class ROGUELIKEFPS_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void InitMenu(UInventory* InInventory, UCraftingSystem* InCraftingSystem, UDataTable* InItemDataTable);

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* CraftModeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DecomposeModeButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModeText;

	UPROPERTY(meta = (BindWidget))
	UCraftingWidget* CraftingPanel;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* InventoryPanel;

	UPROPERTY()
	UInventory* Inventory;

	UPROPERTY()
	UCraftingSystem* CraftingSystem;

	UPROPERTY()
	UDataTable* ItemDataTable;

	UFUNCTION()
	void OnCraftModeClicked();

	UFUNCTION()
	void OnDecomposeModeClicked();

	UFUNCTION()
	void OnCraftingCompleted();
};