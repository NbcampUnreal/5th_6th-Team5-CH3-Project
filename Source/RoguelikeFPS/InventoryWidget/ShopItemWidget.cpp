#include "ShopItemWidget.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Inventory.h"
#include "InventoryWidget.h"

void UShopItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BuyButton)
    {
        BuyButton->OnClicked.AddDynamic(this, &UShopItemWidget::OnBuyClicked);
    }
    if (SellButton)
    {
        SellButton->OnClicked.AddDynamic(this, &UShopItemWidget::OnSellClicked);
    }
}

void UShopItemWidget::SetItemData(UItemBase* InItem)
{
    ItemData = InItem;
    if (ItemNameText && ItemData)
    {
        ItemNameText->SetText(FText::FromName(ItemData->ItemName));
    }
}

void UShopItemWidget::OnBuyClicked()
{
    if (!ItemData)
    {
        return;
    }
    if (AMyCharacter* Player = Cast<AMyCharacter>(GetOwningPlayerPawn()))
    {
        if (Player->Inventory && Player->Inventory->BuyItem(ItemData))
        {
            if (AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController()))
            {
                if (PC->InventoryWidget)
                {
                    PC->InventoryWidget->UpdateUI();
                }
            }
        }
    }
}
void UShopItemWidget::OnSellClicked()
{
    if (!ItemData)
    {
        return;
    }
    if (AMyCharacter* Player = Cast<AMyCharacter>(GetOwningPlayerPawn()))
    {
        if (Player->Inventory && Player->Inventory->SellItem(ItemData, 1))
        {
            if (AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController()))
            {
                if (PC->InventoryWidget)
                {
                    PC->InventoryWidget->UpdateUI();
                }
            }
        }
    }
}