#include "CraftingWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "CraftingSystem.h"
#include "Inventory.h"
#include "ItemData.h"
#include "Engine/DataTable.h"

void UCraftingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ActionButton)
    {
        ActionButton->OnClicked.AddDynamic(this, &UCraftingWidget::OnCraftOrDecomposeClicked);
    }

    PopulateRecipeList();
    UpdateUI();
}

void UCraftingWidget::InitWidget(UCraftingSystem* InCraftingSystem, UInventory* InInventory, UDataTable* InItemDataTable)
{
    CraftingSystem = InCraftingSystem;
    Inventory = InInventory;
    ItemDataTable = InItemDataTable;

    PopulateRecipeList();
    UpdateUI();
}

void UCraftingWidget::SetCraftingMode(ECraftingMode NewMode)
{
    CurrentMode = NewMode;
    PopulateRecipeList();
    UpdateUI();
}

void UCraftingWidget::PopulateRecipeList()
{
    if (!RecipeListBox || !ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Missing RecipeList setup"));
        return;
    }

    RecipeListBox->ClearChildren();
    RecipeButtonMap.Empty();

    const FString Context(TEXT("RecipeList"));
    TArray<FName> RowNames = ItemDataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, Context);
        if (!Row)
        {
            continue;
        }

        if (CurrentMode == ECraftingMode::Craft && Row->CraftingItems.Num() == 0)
        {
            continue;
        }

        if (CurrentMode == ECraftingMode::Decompose && Row->DestroyItems.Num() == 0)
        {
            continue;
        }

        UButton* RecipeButton = NewObject<UButton>(RecipeListBox);
        UTextBlock* Label = NewObject<UTextBlock>(RecipeButton);

        if (Label)
        {
            Label->SetText(FText::FromName(Row->ItemName));
            Label->Font.Size = 14;
            RecipeButton->AddChild(Label);
        }

        RecipeButtonMap.Add(RecipeButton, RowName);

        RecipeButton->OnClicked.AddDynamic(this, &UCraftingWidget::OnRecipeButtonClicked);

        RecipeListBox->AddChild(RecipeButton);
    }

    UE_LOG(LogTemp, Log, TEXT("[CraftingWidget] Recipe list populated (%d items)"), RecipeListBox->GetChildrenCount());
}

void UCraftingWidget::OnRecipeButtonClicked()
{
    UButton* ClickedButton = nullptr;
    for (const TPair<UButton*, FName>& Pair : RecipeButtonMap)
    {
        if (Pair.Key && Pair.Key->HasKeyboardFocus())
        {
            ClickedButton = Pair.Key;
            break;
        }
    }

    if (!ClickedButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Could not determine which button was clicked"));
        return;
    }

    FName* FoundRow = RecipeButtonMap.Find(ClickedButton);
    if (!FoundRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Clicked button not mapped to a recipe"));
        return;
    }

    OnItemSelected(*FoundRow);
}

void UCraftingWidget::OnItemSelected(FName ItemRowName)
{
    SelectedItemName = ItemRowName;

    if (ItemName)
    {
        ItemName->SetText(FText::FromName(ItemRowName));
    }

    UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Selected Item: %s"), *ItemRowName.ToString());
    UpdateUI();
}

void UCraftingWidget::OnCraftOrDecomposeClicked()
{
    if (!CraftingSystem || !Inventory || !ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Missing references"));
        return;
    }

    if (SelectedItemName.IsNone())
    {
        if (CraftStatusText)
        {
            CraftStatusText->SetText(FText::FromString(TEXT("Select an item first")));
        }
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] No item selected"));
        return;
    }

    bool bSuccess = false;

    if (CurrentMode == ECraftingMode::Craft)
    {
        bSuccess = CraftingSystem->CraftItem(Inventory, ItemDataTable, SelectedItemName);
    }
    else
    {
        bSuccess = CraftingSystem->DecomposeItem(Inventory, ItemDataTable, SelectedItemName);
    }

    if (CraftStatusText)
    {
        if (bSuccess)
        {
            CraftStatusText->SetText(FText::FromString(TEXT("Success!")));
        }
        else
        {
            CraftStatusText->SetText(FText::FromString(TEXT("Failed: Missing materials")));
        }
    }

    if (bSuccess)
    {
        OnCraftingCompleted.Broadcast();
    }

    UpdateUI();
}

void UCraftingWidget::UpdateSelectedItemInfo()
{
    if (!ItemDataTable)
    {
        return;
    }

    const FString Context(TEXT("UpdateSelectedItemInfo"));
    FItemData* ItemInfo = ItemDataTable->FindRow<FItemData>(SelectedItemName, Context);

    if (!ItemInfo)
    {
        if (ItemName)
        {
            ItemName->SetText(FText::FromString(TEXT("Select an item")));
        }
        if (ItemImage)
        {
            ItemImage->SetBrushFromTexture(nullptr);
        }
        return;
    }

    if (ItemName)
    {
        ItemName->SetText(FText::FromName(ItemInfo->ItemName));
    }

    if (ItemImage && ItemInfo->Thumbnail)
    {
        ItemImage->SetBrushFromTexture(ItemInfo->Thumbnail);
    }
}

void UCraftingWidget::UpdateUI()
{
    UpdateSelectedItemInfo();
}