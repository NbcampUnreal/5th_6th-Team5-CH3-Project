#include "RoguelikeFPS/InventoryWidget/ShopWidget.h"
#include "RoguelikeFPS/InventoryWidget/ShopItemWidget.h"
#include "Components/ScrollBox.h"
#include "ItemBase.h"
#include "RoguelikeFPS/FPSCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UShopWidget::CloseShop);
    }
}

void UShopWidget::OpenShop(const TArray<UItemBase*>& Items)
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
    SetVisibility(ESlateVisibility::Visible);

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        PC->SetInputMode(InputMode);

        if (AFPSCharacter* Player = Cast<AFPSCharacter>(PC->GetPawn()))
        {
            Player->DisableInput(PC); // 이제 실제 캐릭터에 적용됨
        }
    }
}
void UShopWidget::CloseShop()
{
    // 1. UI 숨기기
    SetVisibility(ESlateVisibility::Hidden);

    if (APlayerController* PC = GetOwningPlayer())
    {
        // 2. 마우스 커서 숨기기 + 게임 모드 입력
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());

        // 3. 캐릭터 입력 복구
        if (AFPSCharacter* Player = Cast<AFPSCharacter>(PC->GetPawn()))
        {
            Player->EnableInput(PC);
        }
    }
}