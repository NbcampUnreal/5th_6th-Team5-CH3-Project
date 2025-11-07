#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacter.h"
#include "StatsHUD.h" // UStatsHUD 포함 (캐스팅을 위해 필요)

AFPSPlayerController::AFPSPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	CrouchAction(nullptr),
	DashAction(nullptr),
	Weapon_FireAction(nullptr),
	Weapon_Fire_AutoAction(nullptr),
	Weapon_ReloadAction(nullptr)
	//,TitleWidgetClass(nullptr) // Title 추가
{
}

// IMC 활성화
void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 1. 입력 매핑 설정
	if (TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
	}
	// 2. HUD 위젯 생성 및 델리게이트 바인딩
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			// 마우스 커서 설정 (게임 중에는 일반적으로 숨김)
			bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			// 3. 캐릭터 참조 획득 및 UStatsHUD에 위젯 설정
			if (AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(GetCharacter()))
			{
				// UStatsHUD로 캐스팅하여 캐릭터를 설정합니다.
				// UStatsHUD는 SetOwningCharacter에서 캐릭터의 델리게이트를 직접 바인딩합니다.
				if (UStatsHUD* StatsHUD = Cast<UStatsHUD>(HUDWidgetInstance))
				{
					StatsHUD->SetOwningCharacter(MyCharacter);
				}

				// **이전의 델리게이트 바인딩 코드는 제거됨**
				/*
				// 스탯 변경 델리게이트 바인딩
				MyCharacter->OnHUDStatChanged.AddDynamic(this, &AFPSPlayerController::OnCharacterStatChanged);

				// 사망 델리게이트 바인딩
				MyCharacter->OnPlayerDeath.AddDynamic(this, &AFPSPlayerController::OnCharacterDied);
				*/
			}
		}
	}
}

/*
// 제거된 함수들: UStatsHUD가 직접 바인딩하므로 제거
void AFPSPlayerController::OnCharacterStatChanged(FName StatName)
{
	// 캐릭터에서 데이터 변경 신호가 오면, HUD 위젯의 업데이트 함수를 호출합니다.
	UpdateHUD(StatName);
}

void AFPSPlayerController::OnCharacterDied(AController* KillerController)
{
	// 사망 시 처리 (예: 게임 오버 화면 표시)
}

void AFPSPlayerController::UpdateHUD(FName StatName)
{
	// 제거됨
}

void AFPSPlayerController::UpdateHUDWidgetStats(FName StatName)
{
	// 제거됨
}
*/