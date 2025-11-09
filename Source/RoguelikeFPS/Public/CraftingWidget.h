#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

class UCraftingSystem;
class UInventory;
class UButton;
class UTextBlock;
class UImage;
class UDataTable;

UENUM(BlueprintType)
enum class ECraftingMode : uint8
{
    Craft UMETA(DisplayName = "Craft"),
    Decompose UMETA(DisplayName = "Decompose")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCraftingCompleted);

UCLASS()
class ROGUELIKEFPS_API UCraftingWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void InitWidget(UCraftingSystem* InCraftingSystem, UInventory* InInventory, UDataTable* InItemDataTable);

    UFUNCTION()
    void OnCraftOrDecomposeClicked();

    UFUNCTION()
    void UpdateUI();

    UFUNCTION(BlueprintCallable)
    void SetCraftingMode(ECraftingMode NewMode);

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCraftingCompleted OnCraftingCompleted;

protected:
    UPROPERTY()
    UCraftingSystem* CraftingSystem;

    UPROPERTY()
    UInventory* Inventory;

    UPROPERTY()
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    FName SelectedItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    ECraftingMode CurrentMode = ECraftingMode::Craft;

    UPROPERTY(meta = (BindWidget))
    UButton* ActionButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ActionButtonText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaterialListText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CraftStatusText;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;
};