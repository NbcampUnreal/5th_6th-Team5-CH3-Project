#include "CraftingMenuWidget.h"
#include "CraftingWidget.h"
#include "InventoryWidget.h"
#include "CraftingSystem.h"
#include "Inventory.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CraftModeButton)
		CraftModeButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnCraftModeClicked);

	if (DecomposeModeButton)
		DecomposeModeButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnDecomposeModeClicked);

	if (CraftingPanel)
		CraftingPanel->OnCraftingCompleted.AddDynamic(this, &UCraftingMenuWidget::OnCraftingCompleted);
}

void UCraftingMenuWidget::InitMenu(UInventory* InInventory, UCraftingSystem* InCraftingSystem, UDataTable* InItemDataTable)
{
	Inventory = InInventory;
	CraftingSystem = InCraftingSystem;
	ItemDataTable = InItemDataTable;

	if (CraftingPanel && Inventory && CraftingSystem)
	{
		CraftingPanel->InitWidget(CraftingSystem, Inventory, ItemDataTable);
		CraftingPanel->SetCraftingMode(ECraftingMode::Craft);
	}

	if (InventoryPanel && Inventory)
	{
		InventoryPanel->InitInventory(Inventory);
		InventoryPanel->UpdateUI();

		InventoryPanel->OnItemClicked.AddDynamic(this, &UCraftingMenuWidget::OnInventoryItemClicked);
	}

	if (ModeText)
	{
		ModeText->SetText(FText::FromString(TEXT("Mode: Craft")));
	}
}

void UCraftingMenuWidget::OnInventoryItemClicked(UItemBase* ClickedItem)
{
	if (!ClickedItem || !CraftingPanel)
	{
		return;
	}
	CraftingPanel->OnItemSelected(ClickedItem->ItemName);

	UE_LOG(LogTemp, Warning, TEXT("[CraftingMenuWidget] Selected item: %s"),
		*ClickedItem->ItemName.ToString());
}

void UCraftingMenuWidget::OnCraftModeClicked()
{
	if (CraftingPanel)
		CraftingPanel->SetCraftingMode(ECraftingMode::Craft);

	if (ModeText)
		ModeText->SetText(FText::FromString(TEXT("Mode: Craft")));
}

void UCraftingMenuWidget::OnDecomposeModeClicked()
{
	if (CraftingPanel)
		CraftingPanel->SetCraftingMode(ECraftingMode::Decompose);

	if (ModeText)
		ModeText->SetText(FText::FromString(TEXT("Mode: Decompose")));
}

void UCraftingMenuWidget::OnCraftingCompleted()
{
	if (InventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CraftingMenuWidget] Crafting completed → Updating inventory"));
		InventoryPanel->UpdateUI();
	}
}