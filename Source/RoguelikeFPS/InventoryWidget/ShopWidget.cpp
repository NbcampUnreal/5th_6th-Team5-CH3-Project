#include "RoguelikeFPS/InventoryWidget/ShopWidget.h"
#include "RoguelikeFPS/InventoryWidget/ShopItemWidget.h"
#include "Components/ScrollBox.h"
#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::InitShop(const TArray<UItemBase*>& Items)
{
    if (!ItemBox)
    {
        return;
    }
    CurrentShopItems = Items;
    ItemBox->ClearChildren();

    for (UItemBase* Item : CurrentShopItems)
    {
        if (!Item)
        {
            continue;
        }
        UShopItemWidget* ItemWidget = CreateWidget<UShopItemWidget>(this, ShopItemWidgetClass);
        if (ItemWidget)
        {
            ItemWidget->SetItemData(Item);
            ItemBox->AddChild(ItemWidget);
        }
    }
}


void UShopWidget::CloseShop()
{
    SetVisibility(ESlateVisibility::Hidden);

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}
