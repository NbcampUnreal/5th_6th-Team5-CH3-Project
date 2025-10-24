#include "MyCharacter.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("InventoryComponent"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Inventory)
	{
		Inventory->TestInventory();
	}
}
