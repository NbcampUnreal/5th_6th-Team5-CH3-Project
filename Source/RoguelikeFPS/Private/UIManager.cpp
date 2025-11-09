#include "UIManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("UIManager Initialized"));
}

void UUIManager::Deinitialize()
{
    Super::Deinitialize();
    HideAllWidgetsExceptLayer(EUILayer::Background);
}

UUserWidget* UUIManager::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, EUILayer Layer)
{
    if (!WidgetClass) return nullptr;

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC) return nullptr;

    UUserWidget* NewWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
    if (NewWidget)
    {
        int32 ZOrder = GetZOrderForLayer(Layer);
        NewWidget->AddToViewport(ZOrder);

        // ActiveWidgetsByClass에 추가
        FWidgetEntry* ClassEntry = ActiveWidgetsByClass.FindByPredicate(
            [WidgetClass](const FWidgetEntry& Entry) { return Entry.WidgetClass == WidgetClass; });
        if (!ClassEntry)
        {
            FWidgetEntry NewEntry;
            NewEntry.WidgetClass = WidgetClass;
            ClassEntry = &ActiveWidgetsByClass.Add_GetRef(NewEntry);
        }
        ClassEntry->Widgets.Add(NewWidget);

        // ActiveWidgetsByLayer에 추가
        FLayerEntry* LayerEntry = ActiveWidgetsByLayer.FindByPredicate(
            [Layer](const FLayerEntry& Entry) { return Entry.Layer == Layer; });
        if (!LayerEntry)
        {
            FLayerEntry NewEntry;
            NewEntry.Layer = Layer;
            LayerEntry = &ActiveWidgetsByLayer.Add_GetRef(NewEntry);
        }
        LayerEntry->Widgets.Add(NewWidget);

        UE_LOG(LogTemp, Log, TEXT("Widget Shown: %s (Layer: %d, ZOrder: %d)"),
            *WidgetClass->GetName(), (uint8)Layer, ZOrder);
    }
    SetInputModeUI();
    return NewWidget;
}

void UUIManager::HideWidget(UUserWidget* Widget)
{
    if (!Widget || !Widget->IsInViewport()) return;
    Widget->RemoveFromParent();

    for (FWidgetEntry& Entry : ActiveWidgetsByClass)
    {
        Entry.Widgets.Remove(Widget);
    }
    for (FLayerEntry& Entry : ActiveWidgetsByLayer)
    {
        Entry.Widgets.Remove(Widget);
    }
    UE_LOG(LogTemp, Log, TEXT("Widget Hidden: %s"), *Widget->GetName());
}

void UUIManager::HideAllWidgetsExceptLayer(EUILayer ExceptLayer)
{
    for (FLayerEntry& Entry : ActiveWidgetsByLayer)
    {
        if (Entry.Layer != ExceptLayer)
        {
            for (UUserWidget* Widget : Entry.Widgets)
            {
                if (Widget)
                {
                    HideWidget(Widget);
                }
            }
            Entry.Widgets.Empty();
        }
    }
}

void UUIManager::HideAllWidgetsExceptClass(TSubclassOf<UUserWidget> ExceptClass)
{
    for (FWidgetEntry& Entry : ActiveWidgetsByClass)
    {
        if (Entry.WidgetClass != ExceptClass)
        {
            for (UUserWidget* Widget : Entry.Widgets)
            {
                if (Widget)
                {
                    HideWidget(Widget);
                }
            }
            Entry.Widgets.Empty();
        }
    }
}

void UUIManager::SetInputModeUI()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeUIOnly());
        PC->bShowMouseCursor = true;
    }
}

void UUIManager::SetInputModeGame()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

bool UUIManager::IsWidgetActive(TSubclassOf<UUserWidget> WidgetClass) const
{
    const FWidgetEntry* Entry = ActiveWidgetsByClass.FindByPredicate(
        [WidgetClass](const FWidgetEntry& Entry) { return Entry.WidgetClass == WidgetClass; });
    return Entry && Entry->Widgets.Num() > 0;
}

void UUIManager::ShowTitleScreen()
{
    // 추가: TitleWidgetClass null 체크
    if (!TitleWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTitleScreen: TitleWidgetClass is NULL! Set it in UUIManager Defaults."));
        return;
    }

    HideAllWidgetsExceptLayer(EUILayer::Background);

    // 추가: World/PC 디버깅
    UWorld* World = GetWorld();
    APlayerController* PC = World ? UGameplayStatics::GetPlayerController(World, 0) : nullptr;
    UE_LOG(LogTemp, Log, TEXT("ShowTitleScreen: World=%s, PC=%s"),
        World ? *World->GetName() : TEXT("NULL"),
        PC ? *PC->GetName() : TEXT("NULL"));

    UUserWidget* TitleWidget = ShowWidget(TitleWidgetClass, EUILayer::Modal);
    if (TitleWidget)
    {
        SetInputModeUI();
        UE_LOG(LogTemp, Log, TEXT("ShowTitleScreen: TitleWidget shown successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTitleScreen: FAILED - Check TitleWidgetClass in Editor, World/PC, or Blueprint validity"));
    }
}

void UUIManager::ShowWeaponSelectMenu()
{
    HideAllWidgetsExceptLayer(EUILayer::Background);
    ShowWidget(MainMenuWidgetClass, EUILayer::Modal);
    SetInputModeUI();
}

int32 UUIManager::GetZOrderForLayer(EUILayer Layer) const
{
    switch (Layer)
    {
    case EUILayer::Background: return 0;
    case EUILayer::Modal:      return 10;
    case EUILayer::Popup:      return 20;
    default:                   return 0;
    }
}