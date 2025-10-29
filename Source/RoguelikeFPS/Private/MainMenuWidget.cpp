#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "GameDataInstance.h"

// Initialize 함수에서 버튼 이벤트 바인딩
bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Button_Weapon1) { Button_Weapon1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon1Clicked); }
	if (Button_Weapon2) { Button_Weapon2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon2Clicked); }
	if (Button_Weapon3) { Button_Weapon3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon3Clicked); }
	if (Button_Weapon4) { Button_Weapon4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon4Clicked); }
	if (Button_Start) { Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked); }
	if (Button_Exit) { Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitGameClicked); }

	return true;
}

// 무기 버튼 래퍼 함수들
void UMainMenuWidget::OnButtonWeapon1Clicked() { HandleWeaponSelection(1); }
void UMainMenuWidget::OnButtonWeapon2Clicked() { HandleWeaponSelection(2); }
void UMainMenuWidget::OnButtonWeapon3Clicked() { HandleWeaponSelection(3); }
void UMainMenuWidget::OnButtonWeapon4Clicked() { HandleWeaponSelection(4); }


void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{
	UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
	if (!GameInstance) return;

	// 재클릭 시 선택 취소 (0으로 설정)
	if (GameInstance->SelectedWeaponIndex == WeaponIndex)
	{
		GameInstance->SetSelectedOption(0);
	}
	// 새로운 무기 선택 시
	else
	{
		GameInstance->SetSelectedOption(WeaponIndex);
	}
}

void UMainMenuWidget::OnStartGameClicked()
{
	UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());

	// 준비 상태 확인
	if (!GameInstance || !GameInstance->bIsReadyToStart)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please select a weapon to start the game."));
		return;
	}

	// 레벨 전환
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_GameMap01"));
}

void UMainMenuWidget::OnExitGameClicked()
{
	// 게임 종료
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}
