#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UItemBase;

UCLASS()
class ROGUELIKEFPS_API UShopItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    void SetItemData(UItemBase* InItem);
    UItemBase* GetItemData() const { return ItemData; }

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;
   
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PriceText;
    
    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    UButton* BuyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SellButton;

    UPROPERTY()
    UItemBase* ItemData;

    UFUNCTION()
    void OnBuyClicked();

    UFUNCTION()
    void OnSellClicked();
};
