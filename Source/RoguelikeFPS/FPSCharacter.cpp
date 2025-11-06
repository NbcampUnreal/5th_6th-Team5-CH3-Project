#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "FPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPSCharacter::AFPSCharacter()		// Initial setup
	: Level(1),
	Health(100),
	MaxHealth(100),
	Attack(10),
	Defence(10),
	AttackSpeed(5),
	MovingSpeed(600),
	Stamina(500),
	Experience(0),
	MaxExperience(100),
	bIsAlive(true)
{
	PrimaryActorTick.bCanEverTick = false; // Tick 비활성화 (필요하면 BeginPlay 또는 생성자에서 true로 변경 필요)


	// Camera Setup
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 50.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// Movement Setup
	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	DashMultifly = 2.0f;
	DashSpeed = MovingSpeed * DashMultifly;
	DashTime = 0.5f;


	// Crouch Activation
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);

	// FireCooltime
	FireCooltime = 2.0f;
	AutoFireTime = 1.0f;

	// Reload Time
	ReloadTime = 1.5f;
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (TObjectPtr<AFPSPlayerController> PlayerController = Cast<AFPSPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::Move
				);

			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::Look
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AFPSCharacter::StopJump
				);
			}

			if (PlayerController->DashAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DashAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::StartDash
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Started,
					this,
					&AFPSCharacter::StartCrouch
				);
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&AFPSCharacter::StopCrouch
				);

			}

			if (PlayerController->Weapon_FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->Weapon_FireAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::Fire
				);
			}

			if (PlayerController->Weapon_Fire_AutoAction)
			{
				EnhancedInput->BindAction(
					PlayerController->Weapon_Fire_AutoAction,
					ETriggerEvent::Started,
					this,
					&AFPSCharacter::StartFire_Auto
				);

				EnhancedInput->BindAction(
					PlayerController->Weapon_Fire_AutoAction,
					ETriggerEvent::Completed,
					this,
					&AFPSCharacter::StopFire_Auto
				);
			}

			if (PlayerController->Weapon_ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->Weapon_ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&AFPSCharacter::Reload
				);
			}
		}
	}
}


void AFPSCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

}
void AFPSCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}
void AFPSCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}
void AFPSCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}

}
void AFPSCharacter::StartDash(const FInputActionValue& value)
{
	if (bIsDashing == true)
	{
		bIsDashing2 = true;
	}
	if (GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DashTimerHandle);
	}

	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	GetWorldTimerManager().SetTimer(
		DashTimerHandle,
		this,
		&AFPSCharacter::StopDash,
		DashTime,
		false
	);

	bIsDashing = true;
}
void AFPSCharacter::StopDash()
{
	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	bIsDashing = false;
	bIsDashing2 = false;
}
void AFPSCharacter::StartCrouch(const FInputActionValue& value)
{
	Crouch();
}
void AFPSCharacter::StopCrouch(const FInputActionValue& value)
{
	UnCrouch();
}
void AFPSCharacter::Fire(const FInputActionValue& value)
{
	bIsFiring = true;

	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Fire!"));

	GetWorldTimerManager().SetTimer(
		FireCooltimeHandle,
		this,
		&AFPSCharacter::StopFire,
		FireCooltime,
		false
	);
}
void AFPSCharacter::StopFire()
{
	bIsFiring = false;
}
void AFPSCharacter::StartFire_Auto(const FInputActionValue& value)
{
	GetWorldTimerManager().SetTimer(
		FireCooltimeHandle,
		this,
		&AFPSCharacter::PerformFire,
		FireCooltime,
		true
	);

}
void AFPSCharacter::PerformFire()
{
	bIsFiring = true;
	GetWorldTimerManager().SetTimer(
		FireCooltimeHandle,
		this,
		&AFPSCharacter::StopFire,
		AutoFireTime,
		false
	);
}
void AFPSCharacter::StopFire_Auto(const FInputActionValue& value)
{
	GetWorldTimerManager().ClearTimer(FireCooltimeHandle);
	bIsFiring = false;
}
void AFPSCharacter::Reload(const FInputActionValue& value)
{
	bIsReloading = true;
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Reloaded!"));
	GetWorldTimerManager().SetTimer(
		ReloadTimeHandle,
		this,
		&AFPSCharacter::StopReload,
		ReloadTime,
		false
	);
}
void AFPSCharacter::StopReload()
{
	bIsReloading = false;
}



void AFPSCharacter::LevelUp()
{
	if (Experience == MaxExperience)
	{
		Level += 1;
		Health += 20;
		Attack += 3;
		Defence += 3;
		Experience = 0;
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		OnLevelUp.Broadcast(PC);
	}
}

void AFPSCharacter::OnDeath(AController* KillerController)
{
	if (Health <= 0) bIsAlive = false;
	OnPlayerDeath.Broadcast(KillerController);
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) - Defence;
	if (ActualDamage > 0)
	{
		Health -= ActualDamage;

		UpdateHUDStats(TEXT("Health"));

		if (Health <= 0.f) OnDeath(EventInstigator);
	}
	return ActualDamage;
}

void AFPSCharacter::ApplyAugment(FName AugmentName)
{
	UE_LOG(LogTemp, Warning, TEXT("ApplyAugment called with: %s"), *AugmentName.ToString());
	// Augment Logic Here (Example: Stat increase, ability unlock)
}
void AFPSCharacter::AddXP(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("Gained XP: %f"), Amount);
	CurrentExperience += Amount;
	UpdateHUDStats(TEXT("Experience"));
	LevelUp();
	// Add XP logic here //레벨업 체크
}

void AFPSCharacter::GainGold(int32 Amount) // 이 함수는 필요하다면 새로 만드세요.
{
	GoldAmount += Amount;

	// 골드 변경 시 HUD 업데이트 신호 전송
	UpdateHUDStats(TEXT("Gold"));
}

void AFPSCharacter::UpdateHUDStats(FName StatName)
{
	// HUD 업데이트 로직이 여기에 추가되어야 합니다.
	// 예: OnHUDStatChanged.Broadcast(StatName);
	OnHUDStatChanged.Broadcast(StatName);
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Tick을 사용하려면 생성자에서 PrimaryActorTick.bCanEverTick = true; 로 변경해야 함

	// 예시: 쿨다운 감소 로직 등
	if (Skill1CooldownRemaining > 0.0f)
	{
		Skill1CooldownRemaining -= DeltaTime;
		if (Skill1CooldownRemaining <= 0.0f)
		{
			OnHUDStatChanged.Broadcast(TEXT("Skill1CD"));
		}
	}
	// ...
}