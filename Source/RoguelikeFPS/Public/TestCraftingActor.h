#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCraftingActor.generated.h"

class UCraftingSystem;
class UDataTable;
class UPartItem;
//테스트용 액터
UCLASS()
class ROGUELIKEFPS_API ATestCraftingActor : public AActor
{
    GENERATED_BODY()
public:
    ATestCraftingActor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    UDataTable* PartDataTable;
};
