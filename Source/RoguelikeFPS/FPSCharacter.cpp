#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "FPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPSCharacter::AFPSCharacter()
	// 초기 스탯 설정
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
	PrimaryActorTick.bCanEverTick = false;

	// 카메라 부착
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 50.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// 대시 스피드
	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	DashMultifly = 10.0f;
	DashSpeed = MovingSpeed * DashMultifly;
	DashTime = 1.0f;

	// Crouch 활성화
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 60.0f;
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
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	if (GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DashTimerHandle);
	}

	// 몇 초 후에 StopDash 함수 호출
	GetWorldTimerManager().SetTimer(
		DashTimerHandle,
		this,
		&AFPSCharacter::StopDash,
		DashTime,
		false
	);
}

void AFPSCharacter::StopDash()
{
	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
}



void AFPSCharacter::StartCrouch(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Crouch Pressed"));
	Crouch();
}

void AFPSCharacter::StopCrouch(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Crouch Stop"));
	UnCrouch();
}

////


// 레벨업 함수
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
}


// 사망 함수
void AFPSCharacter::OnDeath()
{
	if (Health <= 0)
	{
		bIsAlive = false;
		// 사망 시 게임 오버 레벨 호출
	}
}

// 피격 함수
float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, TObjectPtr<AController> EventInstigator, TObjectPtr<AActor> DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) - Defence;  // 실제데미지 = 데미지 - 방어력
	
	if (ActualDamage > 0)
	{
		// 체력 감소
		Health -= ActualDamage;

		if (Health <= 0)
		{
			OnDeath();
		}
	}

	return ActualDamage;
}

