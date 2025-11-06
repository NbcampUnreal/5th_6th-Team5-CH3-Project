#include "ShopItemWidget.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
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
    if (PriceText && ItemData)
    {
        PriceText->SetText(FText::AsNumber(ItemData->BuyPrice));
    }
    if (ItemImage && ItemData)
    {
        ItemImage->SetBrushFromTexture(ItemData->Thumbnail);
    }
    UE_LOG(LogTemp, Warning, TEXT("SetItemData: %s | Thumbnail: %s"),
        *ItemData->ItemName.ToString(),
        ItemData->Thumbnail ? *ItemData->Thumbnail->GetName() : TEXT("NULL"));
}

void UShopItemWidget::OnBuyClicked()
{
    if (!ItemData)
    {
        return;
    }
    if (AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwningPlayerPawn()))
    {
        if (Player->Inventory && Player->Inventory->BuyItem(ItemData))
        {
            if (AFPSPlayerController* PC = Cast<AFPSPlayerController>(Player->GetController()))
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
    if (AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwningPlayerPawn()))
    {
        if (Player->Inventory && Player->Inventory->SellItem(ItemData, 1))
        {
            if (AFPSPlayerController* PC = Cast<AFPSPlayerController>(Player->GetController()))
            {
                if (PC->InventoryWidget)
                {
                    PC->InventoryWidget->UpdateUI();
                }
            }
        }
    }
}