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

	UFUNCTION(BlueprintCallable)
	bool CanUpgrade(UItemBase* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold);

	UFUNCTION(BlueprintCallable)
	bool UpgradeItem(UItemBase* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold);


protected:
	virtual void BeginPlay() override;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float SuccessRate = 0.0f;

    //강화시 능력치 상승
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Bonus")
	float NormalBonusRate = 0.10f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Bonus")
	float RareBonusRate = 0.20f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Bonus")
	float HeroBonusRate = 0.30f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Bonus")
	float LegendaryBonusRate = 0.40f;
};
