#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeSystem.generated.h"

class UPartItem;
class UInventory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKEFPS_API UUpgradeSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUpgradeSystem();

	//테스트용
	void TestPartUpgrade(UInventory* Inventory);

	UFUNCTION(BlueprintCallable)
	bool CanUpgrade(UPartItem* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold);

	UFUNCTION(BlueprintCallable)
	bool UpgradeItem(UPartItem* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float SuccessRate = 0.0f;

protected:
	virtual void BeginPlay() override;		
};
