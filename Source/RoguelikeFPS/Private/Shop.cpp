#include "Shop.h"
#include "Inventory.h"
#include "RoguelikeFPS/FPSCharacter.h"
#include "RoguelikeFPS/FPSPlayerController.h"
#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "RoguelikeFPS/InventoryWidget/ShopItemWidget.h"
#include "RoguelikeFPS/InventoryWidget/ShopWidget.h"



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
    LoadItemsFromDataTable();
}

void AShop::LoadItemsFromDataTable()
{
    ShopItems.Empty();

    if (!ItemDataTable)
    {
        return;
    }
    TArray<FName> RowNames = ItemDataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        if (const FItemData* ItemRow = ItemDataTable->FindRow<FItemData>(RowName, TEXT("ShopItems")))
        {
            UItemBase* NewItem = NewObject<UItemBase>(this);
            NewItem->InitItemData(*ItemRow);

            NewItem->Amount = FMath::Max(1, ItemRow->Amount);

            ShopItems.Add(NewItem);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[Shop] Loaded %d items from data table"), ShopItems.Num());
}

void AShop::PlayerInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
    {
        if (AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(Player->GetController()))
        {
            if (ShopItems.Num() > 0) 
            {
                if (PlayerController->ShopWidget)
                {
                    PlayerController->ShopWidget->OpenShop(ShopItems);
                }
            }
        }
    }
}void AShop::PlayerOutRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
    {
        if (AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(Player->GetController()))
        {
            if (PlayerController->ShopWidget)
            {
                PlayerController->ShopWidget->CloseShop();
            }
        }
    }
}

