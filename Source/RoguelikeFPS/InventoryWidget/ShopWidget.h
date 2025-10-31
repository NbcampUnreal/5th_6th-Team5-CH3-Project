#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UScrollBox;
class UShopItemWidget;
class UItemBase;
class UButton;

UCLASS()
class ROGUELIKEFPS_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitShop(const TArray<UItemBase*>& Items);
		
	UFUNCTION(BlueprintCallable)
	void CloseShop();

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;

	UPROPERTY()
	TArray<UItemBase*> CurrentShopItems;
};
