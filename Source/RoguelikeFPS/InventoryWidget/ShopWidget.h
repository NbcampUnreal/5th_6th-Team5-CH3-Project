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
	void CloseShop();
	
	UFUNCTION(BlueprintCallable)
	void OpenShop(const TArray<UItemBase*>& ShopItems);

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;

	UPROPERTY()
	TArray<UItemBase*> CurrentShopItems;

	virtual void NativeConstruct() override;
};