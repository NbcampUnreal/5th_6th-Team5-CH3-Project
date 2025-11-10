#include "PartWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PartSystem.h"
#include "Inventory.h"
#include "ItemBase.h"

void UPartWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MuzzleEquipButton)
        MuzzleEquipButton->OnClicked.AddDynamic(this, &UPartWidget::OnMuzzleEquipClicked);
    if (MuzzleUnequipButton)
        MuzzleUnequipButton->OnClicked.AddDynamic(this, &UPartWidget::OnMuzzleUnequipClicked);

    if (MagazinEquipButton)
        MagazinEquipButton->OnClicked.AddDynamic(this, &UPartWidget::OnMagazinEquipClicked);
    if (MagazinUnequipButton)
        MagazinUnequipButton->OnClicked.AddDynamic(this, &UPartWidget::OnMagazinUnequipClicked);

    if (StockEquipButton)
        StockEquipButton->OnClicked.AddDynamic(this, &UPartWidget::OnStockEquipClicked);
    if (StockUnequipButton)
        StockUnequipButton->OnClicked.AddDynamic(this, &UPartWidget::OnStockUnequipClicked);

    UpdateUI();
}

void UPartWidget::InitWidget(UPartSystem* InPartSystem, UInventory* InInventory)
{
    PartSystem = InPartSystem;
    LinkedInventory = InInventory;
    UpdateUI();
}

void UPartWidget::UpdateUI()
{
    if (!PartSystem)
        return;

    // 이미지 업데이트
    auto UpdatePartImage = [](UImage* Image, UItemBase* Item)
        {
            if (!Image) return;
            if (Item && Item->Thumbnail)
                Image->SetBrushFromTexture(Item->Thumbnail);
            else
                Image->SetBrushFromTexture(nullptr);
        };

    UpdatePartImage(MuzzleImage, PartSystem->EquippedParts.Muzzle);
    UpdatePartImage(MagazinImage, PartSystem->EquippedParts.Magazin);
    UpdatePartImage(StockImage, PartSystem->EquippedParts.Stock);

    // 상태 텍스트
    if (PartStatusText)
    {
        FString Status;
        Status += FString::Printf(TEXT("Muzzle: %s\n"),
            PartSystem->EquippedParts.Muzzle ? *PartSystem->EquippedParts.Muzzle->ItemName.ToString() : TEXT("None"));
        Status += FString::Printf(TEXT("Magazin: %s\n"),
            PartSystem->EquippedParts.Magazin ? *PartSystem->EquippedParts.Magazin->ItemName.ToString() : TEXT("None"));
        Status += FString::Printf(TEXT("Stock: %s"),
            PartSystem->EquippedParts.Stock ? *PartSystem->EquippedParts.Stock->ItemName.ToString() : TEXT("None"));

        PartStatusText->SetText(FText::FromString(Status));
    }
}

//  각 버튼 클릭
void UPartWidget::OnMuzzleEquipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        UItemBase* Item = LinkedInventory->SearchItemName(TEXT("Muzzle"));
        if (Item)
        {
            PartSystem->EquipPart(Item, LinkedInventory);
            UpdateUI();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PartWidget] No Muzzle part found in inventory."));
        }
    }
}

void UPartWidget::OnMuzzleUnequipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        PartSystem->UnequipPart(EPartType::Muzzle, LinkedInventory);
        UpdateUI();
    }
}

void UPartWidget::OnMagazinEquipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        UItemBase* Item = LinkedInventory->SearchItemName(TEXT("Magazin"));
        if (Item)
        {
            PartSystem->EquipPart(Item, LinkedInventory);
            UpdateUI();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PartWidget] No Magazin part found in inventory."));
        }
    }
}

void UPartWidget::OnMagazinUnequipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        PartSystem->UnequipPart(EPartType::Magazin, LinkedInventory);
        UpdateUI();
    }
}

void UPartWidget::OnStockEquipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        UItemBase* Item = LinkedInventory->SearchItemName(TEXT("Stock"));
        if (Item)
        {
            PartSystem->EquipPart(Item, LinkedInventory);
            UpdateUI();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PartWidget] No Stock part found in inventory."));
        }
    }
}

void UPartWidget::OnStockUnequipClicked()
{
    if (PartSystem && LinkedInventory)
    {
        PartSystem->UnequipPart(EPartType::Stock, LinkedInventory);
        UpdateUI();
    }
}
