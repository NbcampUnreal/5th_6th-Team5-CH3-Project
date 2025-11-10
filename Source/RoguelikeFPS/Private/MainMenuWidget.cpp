#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameDataInstance.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("MainMenuWidget Initialized."));

    if (Button_Weapon1) Button_Weapon1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon1Clicked);
    if (Button_Weapon2) Button_Weapon2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon2Clicked);
    if (Button_Weapon3) Button_Weapon3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon3Clicked);
    if (Button_Weapon4) Button_Weapon4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon4Clicked);
    if (Button_Start)   Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
    if (Button_Exit)    Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackToTitleClicked);

    if (Button_Start) Button_Start->SetIsEnabled(false);

    return true;
}

void UMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            OnMainMenuReady.Broadcast();  // 델리게이트 호출
        });
}

void UMainMenuWidget::OnButtonWeapon1Clicked() { HandleWeaponSelection(1); }
void UMainMenuWidget::OnButtonWeapon2Clicked() { HandleWeaponSelection(2); }
void UMainMenuWidget::OnButtonWeapon3Clicked() { HandleWeaponSelection(3); }
void UMainMenuWidget::OnButtonWeapon4Clicked() { HandleWeaponSelection(4); }

void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{
    UGameDataInstance* GameInstance = UGameDataInstance::GetGameDataInstance(this);
    if (!GameInstance)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ERROR: GameDataInstance is null!"));
        if (Button_Start) Button_Start->SetIsEnabled(false);
        CurrentSelectedWeaponIndex = 0;
        return;
    }

    if (CurrentSelectedWeaponIndex == WeaponIndex)
    {
        CurrentSelectedWeaponIndex = 0;
        GameInstance->SetSelectedOption(0);
        GameInstance->bIsReadyToStart = false;
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Weapon Deselected"));
    }
    else
    {
        CurrentSelectedWeaponIndex = WeaponIndex;
        GameInstance->SetSelectedOption(WeaponIndex);
        GameInstance->bIsReadyToStart = true;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Weapon Selected: %d"), WeaponIndex));
    }

    if (Button_Start)
        Button_Start->SetIsEnabled(GameInstance->bIsReadyToStart);
}

void UMainMenuWidget::OnStartGameClicked()
{
    UGameDataInstance* GameInstance = UGameDataInstance::GetGameDataInstance(GetWorld());
    if (!GameInstance || !GameInstance->bIsReadyToStart) return;

    RemoveFromParent();
    FName LevelName = GameInstance->StageLevelNames.IsValidIndex(0) ? GameInstance->StageLevelNames[0] : FName("L_Map1");



    UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::OnBackToTitleClicked()
{
    RemoveFromParent();

    AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnMainMenuBackClicked();
        OnBackButtonClicked.Broadcast();
    }
}