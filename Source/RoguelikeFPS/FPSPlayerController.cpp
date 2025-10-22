#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"

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
{
}

// IMC È°¼ºÈ­
void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
	}
}
