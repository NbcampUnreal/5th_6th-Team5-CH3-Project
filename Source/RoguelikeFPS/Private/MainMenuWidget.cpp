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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("UMainMenuWidget::Initialize Called!"));

	if (Button_Weapon1) { Button_Weapon1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon1Clicked); }
	if (Button_Weapon2) { Button_Weapon2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon2Clicked); }
	if (Button_Weapon3) { Button_Weapon3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon3Clicked); }
	if (Button_Weapon4) { Button_Weapon4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnButtonWeapon4Clicked); }
	if (Button_Start) { Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked); }
	if (Button_Exit) { Button_Exit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked); }

	return true;
}

// 무기 버튼 래퍼 함수들
void UMainMenuWidget::OnButtonWeapon1Clicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_1"));
	HandleWeaponSelection(1);
}
void UMainMenuWidget::OnButtonWeapon2Clicked() {
	HandleWeaponSelection(2); 
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_2"));
}
void UMainMenuWidget::OnButtonWeapon3Clicked() { 
	HandleWeaponSelection(3);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_3"));
}
void UMainMenuWidget::OnButtonWeapon4Clicked() {
	HandleWeaponSelection(4);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("choised_4"));
}



void UMainMenuWidget::HandleWeaponSelection(int32 WeaponIndex)
{

	UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());
	if (!GameInstance) return;

	// 재클릭 시 선택 취소 (0으로 설정)
	if (GameInstance->SelectedWeaponIndex == WeaponIndex)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("unchoised"));
		GameInstance->SetSelectedOption(0);
	}
	// 새로운 무기 선택 시
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Aderchoise"));
		GameInstance->SetSelectedOption(WeaponIndex);
	}
}

void UMainMenuWidget::OnStartGameClicked()
{
	UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetGameInstance());

	// 1. GameInstance 캐스팅이 실패하는지 확인
	if (!GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, TEXT("FATAL ERROR in Start: GameInstance Cast Failed!"));
		return;
	}

	// 2. bIsReadyToStart의 현재 값을 로그로 출력 (True/False)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		FString::Printf(TEXT("Start Button Clicked. bIsReadyToStart is: %s"), GameInstance->bIsReadyToStart ? TEXT("TRUE") : TEXT("FALSE"))
	);

	// 3. SelectedWeaponIndex 값도 함께 확인
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		FString::Printf(TEXT("Selected Weapon Index is: %d"), GameInstance->SelectedWeaponIndex)
	);

	// 준비 상태 확인
	if (!GameInstance->bIsReadyToStart) // GameInstance는 위에서 이미 확인했으므로 nullptr 검사 생략
	{
		// 이 로그가 계속 출력된다는 것은 GameInstance->bIsReadyToStart가 False라는 뜻
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please select a weapon to start the game."));
		return;
	}
	//위젯 제거
	RemoveFromParent();
	// 레벨 전환
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("SUCCESS! Attempting Level Change..."));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_GameMap01"));
}

void UMainMenuWidget::OnExitButtonClicked()
{
	// 뒤로 가기
	OnBackButtonClicked.Broadcast();
}
