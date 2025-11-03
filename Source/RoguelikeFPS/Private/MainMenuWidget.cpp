#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameDataInstance.h"
#include "Engine/Engine.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("UMainMenuWidget::Initialize Called!"));
    }

    if (Button_Weapon1) Button_Weapon1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon1Clicked);
    if (Button_Weapon2) Button_Weapon2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon2Clicked);
    if (Button_Weapon3) Button_Weapon3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon3Clicked);
    if (Button_Weapon4) Button_Weapon4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon4Clicked);
    if (Button_Start) Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
    if (Button_Exit) Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);

    return true;
}

void UMainMenuWidget::OnButtonWeapon1Clicked()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_1"));
    HandleWeaponSelection(1);
}
void UMainMenuWidget::OnButtonWeapon2Clicked()
{
    HandleWeaponSelection(2);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_2"));
}
void UMainMenuWidget::OnButtonWeapon3Clicked()
{
    HandleWeaponSelection(3);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_3"));
}
void UMainMenuWidget::OnButtonWeapon4Clicked()
{
    HandleWeaponSelection(4);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_4"));
}

void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
    if (!GameInstance)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameInstance cast failed in HandleWeaponSelection"));
        return;
    }

    if (GameInstance->SelectedWeaponIndex == WeaponIndex)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("unchoised"));
        GameInstance->SetSelectedOption(0);
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aderchoise"));
        GameInstance->SetSelectedOption(WeaponIndex);
    }
}

void UMainMenuWidget::OnStartGameClicked()
{
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
    if (!GameInstance)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, TEXT("FATAL ERROR in Start: GameInstance Cast Failed!"));
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            FString::Printf(TEXT("Start Button Clicked. bIsReadyToStart is: %s"), GameInstance->bIsReadyToStart ? TEXT("TRUE") : TEXT("FALSE")));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            FString::Printf(TEXT("Selected Weapon Index is: %d"), GameInstance->SelectedWeaponIndex));
    }

    if (!GameInstance->bIsReadyToStart)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please select a weapon to start the game."));
        return;
    }

    RemoveFromParent();

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("SUCCESS! Attempting Level Change..."));
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_GameMap01"));
}

void UMainMenuWidget::OnExitButtonClicked()
{
    OnBackButtonClicked.Broadcast();
}
