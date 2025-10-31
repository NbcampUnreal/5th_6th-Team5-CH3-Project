#include "MyCharacter.h"
#include "Inventory.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("InventoryComponent"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
    
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}
void AMyCharacter::MoveForward(float Value)
{
    AddMovementInput(FVector::ForwardVector, Value);
}

void AMyCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector::RightVector, Value);
}