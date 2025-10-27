#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "Inventory.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MyPlayerController.generated.h"

UCLASS()
class ROGUELIKEFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	UInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_InventoryOnOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Inventory;

	UFUNCTION(BlueprintCallable)
	void InventoryOnOff();
};
