#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceItem.generated.h"

UCLASS(BlueprintType)
class ROGUELIKEFPS_API UResourceItem : public UObject
{
    GENERATED_BODY()
public:

    UResourceItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 Amount;

};
