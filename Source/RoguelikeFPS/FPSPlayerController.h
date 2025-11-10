#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInventory;
class UInventoryWidget;
class UUpgradeMenuWidget;
class UCraftingWidget;
class UCraftingMenuWidget;
class UShopWidget;
class UPartMenuWidget;
//class UTitleWidget;	//타이틀 테스트

UCLASS()
class ROGUELIKEFPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFPSPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Weapon_FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Weapon_Fire_AutoAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> Weapon_ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> IA_InventoryOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> IA_UpgradeOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> IA_CraftingOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> IA_PartOnOff;


	//인벤토리/상점 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShopWidget> ShopWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	UPROPERTY()
	UShopWidget* ShopWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUpgradeMenuWidget> UpgradeMenuWidgetClass;
	UPROPERTY()
	UUpgradeMenuWidget* UpgradeMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCraftingMenuWidget> CraftingMenuWidgetClass;
	UPROPERTY()
	UCraftingMenuWidget* CraftingMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPartMenuWidget> PartMenuWidgetClass;
	UPROPERTY()
	UPartMenuWidget* PartMenuWidget;

	UFUNCTION(BlueprintCallable)
	void InventoryToggle();	//열기/닫기
	UFUNCTION(BlueprintCallable)
	void UpgradeToggle();
	UFUNCTION(BlueprintCallable)
	void CraftingToggle();
	UFUNCTION(BlueprintCallable)
	void PartToggle();

	void SetUpInputBinding();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")	//Title 테스트
	//TSubclassOf<UTitleWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatsHUDClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidgetInstance;

protected:
	virtual void BeginPlay() override;


};


