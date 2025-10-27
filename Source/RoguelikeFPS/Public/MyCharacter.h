#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class ROGUELIKEFPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventory* Inventory;

protected:
	virtual void BeginPlay() override;
};
