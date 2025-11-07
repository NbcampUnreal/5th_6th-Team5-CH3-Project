// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItem.generated.h"

UENUM(BlueprintType)

enum class EDropItemType : uint8
{
	Goods1 UMETA(DisplayName = "Goods1"),
	Goods2 UMETA(DisplayName = "Goods2"),
	DropItem UMETA(DisplayName = "DropItem"),
	EXP UMETA(DisplayName = "EXP"),
};

class UInventory;

UCLASS()
class ROGUELIKEFPS_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	UInventory* inven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	EDropItemType GoodsType = EDropItemType::Goods1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	int32 Goods1amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	int32 Goods2amount;


	UFUNCTION(BlueprintCallable)
	void DropGoods1(int32 amount);

	UFUNCTION(BlueprintCallable)
	void DropGoods2(int32 amount);

	UFUNCTION(BlueprintCallable)
	void DropTestItem(int32 amount);

	UFUNCTION(BlueprintCallable)
	void Drop(int32 amount);

};
