//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "Making.generated.h"
//
//class UInventory;
//
//USTRUCT(BlueprintType)
//struct FMakingRecipe
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TArray<FName> MakingItems;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName ResultItem;
//};
//
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class ROGUELIKEFPS_API UMaking : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	UMaking();
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Making")
//	TArray<FMakingRecipe> Recipes;
//
//	UPROPERTY()
//	UInventory* PlayerInventory;
//	
//	UFUNCTION()
//	void Init(UInventory* InInventory);
//
//	UFUNCTION(BlueprintCallable, Category = "Crafting")
//	bool CraftItem(const FName& ItemName);
//};