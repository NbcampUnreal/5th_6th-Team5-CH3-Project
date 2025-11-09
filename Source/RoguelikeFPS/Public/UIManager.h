#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

UENUM(BlueprintType)
enum class EUILayer : uint8
{
    Background = 0,   // StatsHUD 등 (항상 유지)
    Modal = 1,        // AugmentWidget, DeathWidget, MainMenuWidget 등
    Popup = 2         // 카운트다운 등
};

USTRUCT()
struct FWidgetEntry
{
    GENERATED_BODY()

    UPROPERTY()
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY()
    TArray<UUserWidget*> Widgets;
};

USTRUCT()
struct FLayerEntry
{
    GENERATED_BODY()

    UPROPERTY()
    EUILayer Layer;

    UPROPERTY()
    TArray<UUserWidget*> Widgets;
};

UCLASS()
class ROGUELIKEFPS_API UUIManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    UUserWidget* ShowWidget(TSubclassOf<UUserWidget> WidgetClass, EUILayer Layer);

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void HideWidget(UUserWidget* Widget);

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void HideAllWidgetsExceptLayer(EUILayer ExceptLayer);

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void HideAllWidgetsExceptClass(TSubclassOf<UUserWidget> ExceptClass);

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void SetInputModeUI();

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void SetInputModeGame();

    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    bool IsWidgetActive(TSubclassOf<UUserWidget> WidgetClass) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowTitleScreen();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowWeaponSelectMenu();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> TitleWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> StatsHUDClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> AugmentWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> DeathWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameClearWidgetClass;

    UPROPERTY()
    TArray<FWidgetEntry> ActiveWidgetsByClass;

    UPROPERTY()
    TArray<FLayerEntry> ActiveWidgetsByLayer;

    int32 GetZOrderForLayer(EUILayer Layer) const;

private:
    TMap<EUILayer, TArray<TObjectPtr<UUserWidget>>> ActiveWidgets;
};