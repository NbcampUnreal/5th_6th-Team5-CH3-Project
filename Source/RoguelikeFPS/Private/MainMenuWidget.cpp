#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameDataInstance.h"
#include "FPSGameMode.h" // GameMode 접근을 위해 포함 (OnExitButtonClicked에서 사용)

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

    // Title로 돌아가는 버튼 (OnExitButtonClicked 사용)
    if (Button_Exit) Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);

    // 시작 시 Start 버튼 비활성화 (무기 선택 전)
    if (Button_Start)
        Button_Start->SetIsEnabled(false);

    return true;
}

// ===================== 무기 선택 래퍼 ===================== //
void UMainMenuWidget::OnButtonWeapon1Clicked() { HandleWeaponSelection(1); }
void UMainMenuWidget::OnButtonWeapon2Clicked() { HandleWeaponSelection(2); }
void UMainMenuWidget::OnButtonWeapon3Clicked() { HandleWeaponSelection(3); }
void UMainMenuWidget::OnButtonWeapon4Clicked() { HandleWeaponSelection(4); }

// ===================== 공통 무기 선택 로직 ===================== //
void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
    if (!GameInstance)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameInstance Cast Failed."));
        if (Button_Start) Button_Start->SetIsEnabled(false);
        CurrentSelectedWeaponIndex = 0;
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

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Weapon Selected: %d"), WeaponIndex));
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

    // GameInstance에서 다음 맵 이름을 가져와서 로딩 (하드코딩 방지)
    FName LevelToOpen = GameInstance->StageLevelNames.IsValidIndex(0) ? GameInstance->StageLevelNames[0] : TEXT("L_GameMap1");

    UGameplayStatics::OpenLevel(GetWorld(), LevelToOpen);
}

// ===================== 타이틀로 돌아가기 버튼 (ExitButton 사용) ===================== //
void UMainMenuWidget::OnExitButtonClicked()
{
    // Title 위젯을 다시 띄우도록 GameMode에 요청
    AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (FPSGameMode)
    {
        // Title 위젯을 다시 띄우는 로직을 GameMode에서 처리하도록 호출
        FPSGameMode->OnMainMenuBackClicked();
    }

    RemoveFromParent();
}