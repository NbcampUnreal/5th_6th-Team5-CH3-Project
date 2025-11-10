#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartMenuWidget.generated.h"

class UInventoryWidget;
class UPartWidget;
class UInventory;
class UPartSystem;
class UItemBase;

UCLASS()
class ROGUELIKEFPS_API UPartMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void InitMenu(UInventory* InInventory, UPartSystem* InPartSystem);

protected:
    // 인벤토리 위젯
    UPROPERTY(meta = (BindWidget))
    UInventoryWidget* InventoryWidget;

    // 파츠 위젯
    UPROPERTY(meta = (BindWidget))
    UPartWidget* PartWidget;

    // 참조
    UPROPERTY()
    UInventory* InventoryRef;

    UPROPERTY()
    UPartSystem* PartSystemRef;

    // 아이템 클릭시 호출될 함수
    UFUNCTION()
    void OnInventoryItemClicked(UItemBase* ClickedItem);
};
