#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameDataInstance.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("MainMenuWidget Initialized."));

    // 버튼 클릭 이벤트 바인딩
    if (Button_Weapon1) Button_Weapon1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon1Clicked);
    if (Button_Weapon2) Button_Weapon2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon2Clicked);
    if (Button_Weapon3) Button_Weapon3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon3Clicked);
    if (Button_Weapon4) Button_Weapon4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon4Clicked);

    if (Button_Start) Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
    if (Button_Exit) Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackToTitleClicked);

    // 시작 시 Start 버튼 비활성화 (무기 선택 전)
    if (Button_Start)
        Button_Start->SetIsEnabled(false);

    return true;
}

//  무기 선택 래퍼 
void UMainMenuWidget::OnButtonWeapon1Clicked() { HandleWeaponSelection(1); }
void UMainMenuWidget::OnButtonWeapon2Clicked() { HandleWeaponSelection(2); }
void UMainMenuWidget::OnButtonWeapon3Clicked() { HandleWeaponSelection(3); }
void UMainMenuWidget::OnButtonWeapon4Clicked() { HandleWeaponSelection(4); }

// 공통 무기 선택 로직 
void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());

    if (!GameInstance)
    {
        // 오류 메시지를 더 명확하게 변경
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
            TEXT("ERROR: GetGameInstance() is NOT UGameDataInstance! Check Project Settings -> Maps & Modes."));

        // 이 시점에서 Start 버튼 활성화/비활성화가 무의미해지므로 종료
        if (Button_Start) Button_Start->SetIsEnabled(false);
        CurrentSelectedWeaponIndex = 0; // 안전하게 초기화
        return;
    }

    // 같은 무기를 다시 누르면 선택 해제
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
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("Weapon Selected: %d"), WeaponIndex));
    }
    // Start 버튼 활성화 / 비활성화
    if (Button_Start)
        Button_Start->SetIsEnabled(GameInstance->bIsReadyToStart);
}

// ===================== 게임 시작 버튼 ===================== //
void UMainMenuWidget::OnStartGameClicked()
{
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
    if (!GameInstance || !GameInstance->bIsReadyToStart)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Cannot start game"));
        return;
    }

    RemoveFromParent();
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Map1"));
}

// ===================== 뒤로가기 버튼 ===================== //
void UMainMenuWidget::OnBackToTitleClicked()
{
    RemoveFromParent();
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
    AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (FPSGameMode)
    {
        FPSGameMode->OnMainMenuBackClicked();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameMode Cast Failed in MainMenuWidget::OnBackToTitleClicked"));
    }
}