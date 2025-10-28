#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMyCharacter;
class UItemBase;

UCLASS()
class ROGUELIKEFPS_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	AShop();

	UFUNCTION(BlueprintCallable)
	bool BuyItem(AMyCharacter* Player, UItemBase* Item);
	UFUNCTION(BlueprintCallable)
	bool SellItem(AMyCharacter* Player, UItemBase* Item);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	UBoxComponent* InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
	UStaticMeshComponent* ShopMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<UItemBase*> ShopItems;

	UFUNCTION()
	void PlayerInRange(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void PlayerOutRange(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
