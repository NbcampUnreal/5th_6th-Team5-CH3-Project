#include "FPSCharacter.h"
#include "Inventory.h"
#include "UpgradeSystem.h"
#include "CraftingSystem.h"
#include "Engine/DataTable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "FPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AugmentWidget.h"
#include "FPSGameMode.h"
#include "StatsHUD.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// GETTER 구현 (모두 헤더에 인라인으로 선언되어 있으므로 이 파일에 구현이 없습니다.)
// int32 AFPSCharacter::GetLevel() { return Level; }
// ... 등

// SETTER IMPLEMENTATIONS (헤더에 인라인으로 선언되어 있으므로 이 파일에 구현이 없습니다.)
void AFPSCharacter::SetLevel(int32 level) { Level = level; }
void AFPSCharacter::SetHealth(int32 health) { Health = health; }
void AFPSCharacter::SetMaxHealth(int32 maxHealth) { MaxHealth = maxHealth; }
void AFPSCharacter::SetAttack(int32 attack) { Attack = attack; }
void AFPSCharacter::SetDefence(int32 defence) { Defence = defence; }
void AFPSCharacter::SetShield(int32 shield) { Shield = shield; }
void AFPSCharacter::SetAttackSpeed(int32 attackSpeed) { AttackSpeed = attackSpeed; }
void AFPSCharacter::SetMovingSpeed(int32 movingSpeed) { MovingSpeed = movingSpeed; }
void AFPSCharacter::SetStamina(int32 stamina) { Stamina = stamina; }
void AFPSCharacter::SetExperience(int32 experience) { Experience = experience; }
void AFPSCharacter::SetMaxExperience(int32 maxExperience) { MaxExperience = maxExperience; }
void AFPSCharacter::SetIsDashing(bool isdash) { bIsDashing = isdash; }


// CONSTRUCTOR
AFPSCharacter::AFPSCharacter()		// 초기 설정
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
	bIsAlive(true),
	Shield(100) // Shield 초기값 설정
{
	PrimaryActorTick.bCanEverTick = true;


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

	//인벤토리 부착
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("InventoryComponent"));
	UpgradeSystem = CreateDefaultSubobject<UUpgradeSystem>(TEXT("UpgradeSystem"));

	// Crouch Activation
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);

	// FireCooltime
	FireCooltime = 2.0f;
	AutoFireTime = 1.0f;

	// Reload Time
	ReloadTime = 1.5f;

	//(임시)
	static ConstructorHelpers::FObjectFinder<UInputAction> LevelUpTestFinder(TEXT("/Game/Input/Actions/IA_LevelUPTest"));
	if (LevelUpTestFinder.Succeeded())
	{
		LevelUpTestAction = LevelUpTestFinder.Object;
	}
}
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if (Inventory && UpgradeSystem)
	//{
	//	UpgradeSystem->TestPartUpgrade(Inventory);
	//}
}


// INPUT BINDING
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

// INPUT ACTION IMPLEMENTATIONS
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
	if (GetCharacterMovement()->IsFalling()) return; 
	if (value.Get<bool>()) {
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


// GAMEPLAY LOGIC IMPLEMENTATIONS
void AFPSCharacter::LevelUp()
{	
	if (!bIsAlive) //사망 상태일 때
	{
		UE_LOG(LogTemp, Warning, TEXT("사망 상태: 레벨업 불가"));
		return;
	}
	if (Experience >= MaxExperience)
	{
		Level += 1;
		Health += 20;
		Attack += 3;
		Defence += 3;
		CurrentExperience = 0.0f;
		Experience = 0;
		MaxExperience *= 1.2f; // 다음 레벨 요구 경험치 증가
		UpdateHUDStats(TEXT("Health"));
		UpdateHUDStats(TEXT("Attack"));
		UpdateHUDStats(TEXT("Defence"));
		UpdateHUDStats(TEXT("Experience"));
		UpdateHUDStats(TEXT("Level"));

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->HandlePlayerLevelUp(PC);
			}
		}
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			OnLevelUp.Broadcast(PC);
		}
	}
}

void AFPSCharacter::OnDeath(AController* KillerController)
{
	if (Health <= 0) bIsAlive = false;
	OnPlayerDeath.Broadcast(KillerController);
}

// 무적상태 함수
void AFPSCharacter::OnUndead()
{
	Undead = true;
}
void AFPSCharacter::OffUndead()
{
	Undead = false;
}

// 무적상태 함수 타이머 (Undead를 True로 변경하고, UndeadTime이 지나면 Undead = false.) 
void AFPSCharacter::OnUndeadTime()
{
	Undead = true;

	GetWorldTimerManager().SetTimer(
		UndeadTimeHandle,
		this,
		&AFPSCharacter::OffUndead,
		UndeadTime,
		false
	);
}

// 무적상태가 아닐 때 데미지를 적용
float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) - Defence;
	if (Undead == true && ActualDamage > 0)
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
	AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode || !GameMode->AugmentDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("증강 데이터 테이블을 찾을 수 없습니다!"));
		return;
	}
	if (AppliedAugments.Contains(AugmentName))
	{
		UE_LOG(LogTemp, Warning, TEXT("증강 %s 이미 적용됨!"), *AugmentName.ToString());
		return;
	}
	FAugmentData* AugmentData = GameMode->AugmentDataTable->FindRow<FAugmentData>(AugmentName, TEXT(""));
	if (AugmentData)
	{
		if (AugmentData->HealthBonus != 0)
		{
			SetMaxHealth(GetMaxHealth() + AugmentData->HealthBonus);
			SetHealth(GetHealth() + AugmentData->HealthBonus);
			UpdateHUDStats(TEXT("Health"));
		}
		if (AugmentData->AttackBonus != 0)
		{
			SetAttack(GetAttack() + AugmentData->AttackBonus);
			UpdateHUDStats(TEXT("Attack"));
		}
		if (AugmentData->DefenseBonus != 0)
		{
			SetDefence(GetDefence() + AugmentData->DefenseBonus);
			UpdateHUDStats(TEXT("Defence"));
		}
		if (AugmentData->ShieldBonus != 0)
		{
			SetShield(GetShield() + AugmentData->ShieldBonus);
			UpdateHUDStats(TEXT("Shield"));
		}
		if (AugmentData->AttackSpeedMultiplier != 1.0f)
		{
			SetAttackSpeed(GetAttackSpeed() * AugmentData->AttackSpeedMultiplier);
			UpdateHUDStats(TEXT("AttackSpeed"));
		}
		if (AugmentData->MovingSpeedMultiplier != 1.0f)
		{
			SetMovingSpeed(GetMovingSpeed() * AugmentData->MovingSpeedMultiplier);
			GetCharacterMovement()->MaxWalkSpeed = GetMovingSpeed();
			UpdateHUDStats(TEXT("MovingSpeed"));
		}
		AppliedAugments.Add(AugmentName);
		UE_LOG(LogTemp, Log, TEXT("증강 적용됨: %s"), *AugmentName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("증강 %s 데이터 테이블에서 찾을 수 없음!"), *AugmentName.ToString());
	}
}

void AFPSCharacter::AddXP(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("Gained XP: %f"), Amount);
	CurrentExperience += Amount;
	Experience = static_cast<int32>(CurrentExperience);
	UpdateHUDStats(TEXT("Experience"));
	LevelUp();
}

void AFPSCharacter::GainGold(int32 Amount)
{
	GoldAmount += Amount;
	// 골드 변경 시 HUD 업데이트 신호 전송
	UpdateHUDStats(TEXT("Gold"));
}

void AFPSCharacter::UpdateHUDStats(FName StatName)
{
	if (AFPSPlayerController* PC = Cast<AFPSPlayerController>(GetController()))
	{	
		if (UStatsHUD* StatsHUD = Cast<UStatsHUD>(PC->HUDWidgetInstance))
		{
			if (StatName == TEXT("Level"))
			{
				FName FunctionName = TEXT("UpdateLevelText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &Level);
				}
			}
			else if (StatName == TEXT("Experience"))
			{
				FName FunctionName = TEXT("UpdateEXPProgress");
				if (StatsHUD->FindFunction(FunctionName))
				{
					struct FEXPParams
					{
						float CurrentEXP;
						float MaxEXP;
					} Params{ CurrentExperience, static_cast<float>(MaxExperience) };
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &Params);
				}
			}
			if (StatName == TEXT("Health"))
			{
				FName FunctionName = TEXT("UpdateHealthText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &Health);
				}
			}
			if (StatName == TEXT("CurrentAmmo"))
			{
				FName FunctionName = TEXT("UpdateCurrentAmmoText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &CurrentAmmo);
				}
			}
			if (StatName == TEXT("MaxAmmo"))
			{
				FName FunctionName = TEXT("UpdateMaxAmmoText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &MaxAmmo);
				}
			}
			if (StatName == TEXT("Skill1CooldownRemaining"))
			{
				FName FunctionName = TEXT("UpdateSkill1CooldownRemainingText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &Skill1CooldownRemaining);
				}
			}
			if (StatName == TEXT("Skill2CooldownRemaining"))
			{
				FName FunctionName = TEXT("UpdateSkill2CooldownRemainingText");
				if (StatsHUD->FindFunction(FunctionName))
				{
					StatsHUD->ProcessEvent(StatsHUD->FindFunction(FunctionName), &Skill2CooldownRemaining);
				}
			}
			// 기존 스탯 업데이트
			OnHUDStatChanged.Broadcast(StatName);
		}
	}
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
}