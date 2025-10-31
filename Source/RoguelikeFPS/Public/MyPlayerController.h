#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MyPlayerController.generated.h"

class UInventoryWidget;
class UInventory;
class UShopItemWidget;
class UItemBase;
class UShopWidget;

UCLASS()
class ROGUELIKEFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopItemWidget> ShopItemWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopWidget> ShopWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	UPROPERTY()
	UShopWidget* ShopWidget;
	

	UPROPERTY()
	UInventory* PlayerInventory;
	
	//입력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_InventoryOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Inventory;

	//인벤토리 열기/닫기
	UFUNCTION(BlueprintCallable)
	void InventoryOnOff();
	//상점 열기
	UFUNCTION(BlueprintCallable)
	void OpenShop(const TArray<UItemBase*>& ShopItems);

};
