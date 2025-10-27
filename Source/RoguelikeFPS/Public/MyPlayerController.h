#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "Inventory.h"
#include "MyPlayerController.generated.h"

UCLASS()
class ROGUELIKEFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	UInventory* PlayerInventory;

	UFUNCTION(BlueprintCallable)
	void InventoryOnOff();
};
