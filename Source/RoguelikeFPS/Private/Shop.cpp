#include "Shop.h"
#include "Inventory.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "ItemBase.h"
#include "ItemData.h"
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
    if (!ItemDataTable) return;

    TArray<FName> RowNames = ItemDataTable->GetRowNames();
    for (const FName& Name : RowNames)
    {
        FItemData* Row = ItemDataTable->FindRow<FItemData>(Name, TEXT("ShopItems"));
        if (Row)
        {
            UItemBase* NewItem = NewObject<UItemBase>(this);
            NewItem->ItemNumber = Row->ItemNumber;
            NewItem->ItemName = Row->ItemName;
            NewItem->Amount = Row->Amount;
            NewItem->BuyPrice = Row->BuyPrice;
            NewItem->SellPrice = Row->SellPrice;
            NewItem->Thumbnail = Row->Thumbnail;
            ShopItems.Add(NewItem);
        }
    }
}

void AShop::PlayerInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        if (AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController()))
        {
            if (ShopItems.Num() > 0)
            {
                PC->OpenShop(ShopItems);
            }
        }
    }
}void AShop::PlayerOutRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Player->GetController()))
        {
            if (PlayerController->ShopWidget)
            {
                PlayerController->ShopWidget->SetVisibility(ESlateVisibility::Hidden);
                PlayerController->bShowMouseCursor = false;
                PlayerController->SetInputMode(FInputModeGameOnly());
            }
        }
    }
}

