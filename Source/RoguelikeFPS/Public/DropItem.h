// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItem.generated.h"

UENUM(BlueprintType)

enum class EDropItemType : uint8
{
	Gold UMETA(DisplayName = "Gold"),
	Stone UMETA(DisplayName = "Stone"),
	Parts UMETA(DisplayName = "Parts"),
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
	EDropItemType GoodsType = EDropItemType::Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryGold")
	int32 ADDAmount;

	UFUNCTION(BlueprintCallable)
	int32 GetADDAmount() { return ADDAmount; }

	UFUNCTION(BlueprintCallable)
	void SetADDAmount(int32 amount) { ADDAmount = amount; }

	UFUNCTION(BlueprintCallable)
	void DropGold();

	UFUNCTION(BlueprintCallable)
	void DropStone();

	UFUNCTION(BlueprintCallable)
	void DropParts();

	UFUNCTION(BlueprintCallable)
	void Drop();

};
