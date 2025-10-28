#include "Shop.h"
#include "Inventory.h"
#include "MyCharacter.h"
#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"

AShop::AShop()
{
    PrimaryActorTick.bCanEverTick = false;

    ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));
    RootComponent = ShopMesh;

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetBoxExtent(FVector(200.f));
    InteractionBox->SetCollisionProfileName("Trigger");

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AShop::PlayerInRange);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AShop::PlayerOutRange);

}

void AShop::BeginPlay()
{
    Super::BeginPlay();

}

void AShop::PlayerInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Log, TEXT("Player In Range!"));
    }
}

void AShop::PlayerOutRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Log, TEXT("Player Out Range!"));
    }
}

bool AShop::BuyItem(AMyCharacter* Player, UItemBase* Item)
{
    if (!Player || !Item)
    {
        return false;
    }
    if (Player->Inventory->Gold >= Item->BuyPrice)
    {
        Player->Inventory->AddItem(Item);
        Player->Inventory->Gold -= Item->BuyPrice;

        UE_LOG(LogTemp, Log, TEXT("BuyItem : %s, Current Gold : %d"), *Item->ItemName.ToString(), Player->Inventory->Gold);
        return true;
    }
    return false;
}
bool AShop::SellItem(AMyCharacter* Player, UItemBase* Item)
{
    if (!Player || !Item)
    {
        return false;
    }
    if (Player->Inventory->RemoveItem(Item))
    {
        Player->Inventory->Gold += Item->SellPrice;
        UE_LOG(LogTemp, Log, TEXT("SoldItem : %s, Current Gold : %d"), *Item->ItemName.ToString(), Player->Inventory->Gold);
        return true;
    }
    return false;
}

