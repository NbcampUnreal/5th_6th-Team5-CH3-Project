#include "UpgradeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "UpgradeSystem.h"
#include "Inventory.h"
#include "InventoryWidget.h"
#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"

void UUpgradeWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UpgradeButton)
    {
        UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeWidget::OnUpgradeClicked);
    }
}

void UUpgradeWidget::InitWidget(UUpgradeSystem* InUpgradeSystem, UInventory* InInventory)
{
    UpgradeSystem = InUpgradeSystem;
    Inventory = InInventory;
    if (Inventory)
    {
        Inventory->OnInventoryUpdated.AddDynamic(this, &UUpgradeWidget::OnInventoryUpdated);
        Inventory->OnGoldChanged.AddDynamic(this, &UUpgradeWidget::OnGoldChanged);

    }

    UpdateUI();
}

void UUpgradeWidget::BindInventory(UInventoryWidget* InventoryWidget)
{
    if (InventoryWidget)
    {
        InventoryWidget->OnItemClicked.AddDynamic(this, &UUpgradeWidget::OnItemSelected);
    }
}

void UUpgradeWidget::OnItemSelected(UItemBase* SelectedItem)
{
    if (!SelectedItem || !Inventory)
    {
        return;
    }

    if (SelectedItem->ItemType == EItemType::PartItem)
    {
        SelectedPart = SelectedItem;

        if (TargetItemName)
        {
            TargetItemName->SetText(FText::FromName(SelectedPart->ItemName));
        }
        if (TargetItemGrade)
        {
            TargetItemGrade->SetText(FText::FromString(TEXT("Selected Part!")));
        }
        if (TargetItemImage && SelectedPart->Thumbnail)
        {
            TargetItemImage->SetBrushFromTexture(SelectedPart->Thumbnail);
        }
    }

    else if (SelectedItem->ItemName == "강화돌" || SelectedItem->ItemType == EItemType::MaterialItem)
    {
        if (!SelectedPart)
        {
            if (TargetItemGrade)
            {
                TargetItemGrade->SetText(FText::FromString(TEXT("Selected UpgradeItem")));
            }
            return;
        }

        UpgradeMaterial = SelectedItem;
        UE_LOG(LogTemp, Warning, TEXT("[UpgradeWidget] 강화 재료 선택: %s"), *UpgradeMaterial->ItemName.ToString());

        if (MaterialName)
        {
            MaterialName->SetText(FText::FromName(UpgradeMaterial->ItemName));
        }

        if (MaterialImage && UpgradeMaterial->Thumbnail)
        {
            MaterialImage->SetBrushFromTexture(UpgradeMaterial->Thumbnail);
        }
    }

    UpdateUI();
}

void UUpgradeWidget::OnUpgradeClicked()
{
    if (!SelectedPart || !UpgradeSystem || !Inventory)
    {
        return;
    }
    const bool bSuccess = UpgradeSystem->UpgradeItem(SelectedPart, UpgradeMaterial, Inventory, GoldCost);
    
    if (bSuccess)
    {
        TargetItemGrade->SetText(FText::FromString(TEXT("Upgrade Success!")));
        UE_LOG(LogTemp, Warning, TEXT("[UpgradeWidget] Upgrade Success -> %s (New Grade: %d)"),
            *SelectedPart->ItemName.ToString(),
            (uint8)SelectedPart->PartGrade);
    }
    else
    {
        TargetItemGrade->SetText(FText::FromString(TEXT("Upgrade Failed..")));
        UE_LOG(LogTemp, Warning, TEXT("[UpgradeWidget] Upgrade Failed... Grade: %d"), (uint8)SelectedPart->PartGrade);
    }

    UpdateUI();
}

void UUpgradeWidget::OnGoldChanged(int32 NewGold)
{
    UpdateUI();
}

void UUpgradeWidget::OnInventoryUpdated()
{
    UE_LOG(LogTemp, Warning, TEXT("[UpgradeWidget] OnInventoryUpdated triggered!"));
    UpdateUI();
}

void UUpgradeWidget::UpdateUI()
{
    if (!Inventory) return;

    if (GoldCostText)
    {
        GoldCostText->SetText(FText::Format(FText::FromString(TEXT("Current Gold: {0} G")), FText::AsNumber(Inventory->GetGold())));
    }
    if (UpgradeMaterial)
    {
        UItemBase* Mat = Inventory->SearchItemName(UpgradeMaterial->ItemName);

        if (!Mat || Mat->Amount <= 0)
        {
            UpgradeMaterial = nullptr;

            if (MaterialImage && MaterialName && MaterialCount)
            {
                MaterialImage->SetBrushFromTexture(nullptr);
                MaterialName->SetText(FText::FromString(TEXT("No material")));
                MaterialCount->SetText(FText::FromString(TEXT("x 0")));
            }
        }
        else
        {
            if (MaterialCount && MaterialName && MaterialImage && Mat->Thumbnail)
            {
                MaterialCount->SetText(FText::Format(FText::FromString(TEXT("x {0}Amount")), FText::AsNumber(Mat->Amount)));
                MaterialName->SetText(FText::FromName(Mat->ItemName));
                MaterialImage->SetBrushFromTexture(Mat->Thumbnail);
            }
        }
    }
    else
    {
        if (MaterialName && MaterialCount && MaterialImage)
        {
            MaterialName->SetText(FText::FromString(TEXT("No material")));
            MaterialCount->SetText(FText::FromString(TEXT("x 0")));
            MaterialImage->SetBrushFromTexture(nullptr);
        }
    }
    if (MaterialCount && UpgradeMaterial)
    {
        UItemBase* Mat = Inventory->SearchItemName(UpgradeMaterial->ItemName);
        int32 Count = Mat ? Mat->Amount : 0;
        MaterialCount->SetText(FText::Format(FText::FromString(TEXT("{0}Amount")), FText::AsNumber(Count)));
    }

    if (SuccessRateText && SelectedPart)
    {
        float BaseRate = 80.0f; 
        float GradePenalty = (float)((uint8)SelectedPart->PartGrade) * 15.0f; 
        float FinalRate = FMath::Clamp(BaseRate - GradePenalty, 10.0f, 100.f);
        SuccessRateText->SetText(FText::Format(FText::FromString(TEXT("probability of success: {0}%")), FText::AsNumber(FinalRate)));
    }

    if (UpgradeButton)
    {
        bool bHasGold = Inventory->GetGold() >= GoldCost;
        bool bHasMaterial = UpgradeMaterial != nullptr;
        bool bHasPart = SelectedPart != nullptr;
        UpgradeButton->SetIsEnabled(SelectedPart && bHasGold && bHasMaterial);
    }
}