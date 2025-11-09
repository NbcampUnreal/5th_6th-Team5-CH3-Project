#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UItemBase;
class DataTable;

UCLASS()
class ROGUELIKEFPS_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	AShop();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	UBoxComponent* InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
	UStaticMeshComponent* ShopMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<UItemBase*> ShopItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	UDataTable* ItemDataTable;
	
	UFUNCTION()
	void LoadItemsFromDataTable();

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
