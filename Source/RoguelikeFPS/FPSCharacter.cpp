// FPSCharacter.cpp

#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h" 
#include "DeathWidget.h" 
#include "Blueprint/UserWidget.h" 
#include "Kismet/GameplayStatics.h"
// Enhanced Input 관련 헤더 모두 제거됨


AFPSCharacter::AFPSCharacter()
// 초기 스탯 설정 (유지)
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

	// 카메라 부착 (유지)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 50.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// 대시 스피드
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
		GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

		GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
	}

	DashMultifly = 10.0f;
	DashSpeed = MovingSpeed * DashMultifly;
	DashTime = 1.0f;
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!DeathWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeathWidgetClass is not set on %s"), *GetName());
	}
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AFPSCharacter::MoveForward(float AxisValue)
{
	if (!Controller || FMath::IsNearlyZero(AxisValue)) return;
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void AFPSCharacter::MoveRight(float AxisValue)
{
	if (!Controller || FMath::IsNearlyZero(AxisValue)) return;
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void AFPSCharacter::LookYaw(float AxisValue)
{
	if (!Controller || FMath::IsNearlyZero(AxisValue)) return;
	AddControllerYawInput(AxisValue);
}

void AFPSCharacter::LookPitch(float AxisValue)
{
	if (!Controller || FMath::IsNearlyZero(AxisValue)) return;
	AddControllerPitchInput(AxisValue);
}


void AFPSCharacter::StartJump()
{
	Jump();
}

void AFPSCharacter::StopJump()
{
	StopJumping();
}


void AFPSCharacter::StartDash()
{
	if (!bIsAlive || !GetCharacterMovement()) return;

	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	if (GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DashTimerHandle);
	}

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
	if (!bIsAlive || !GetCharacterMovement()) return;

	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
}



void AFPSCharacter::StartCrouch()
{
	Crouch();
}


void AFPSCharacter::StopCrouch()
{
	UnCrouch();
}


// 레벨업 함수 (유지)
void AFPSCharacter::LevelUp()
{
	if (Experience >= MaxExperience)
	{
		Level += 1;
		Health += 20;
		Attack += 3;
		Defence += 3;
		Experience = 0;
	}
}

// 아이템 (임시)
void AFPSCharacter::Meal(int32 MealAmount)
{
	if (!bIsAlive) return;

	// **[추가]** 최대 체력을 초과하지 않도록 체력 회복
	Health = FMath::Min(Health + MealAmount, MaxHealth);

	// 필요하다면 디버그 로그 추가
	// UE_LOG(LogTemp, Warning, TEXT("Healed by Meal: %d. Current Health: %d"), MealAmount, Health);
}

// 사망 함수
void AFPSCharacter::OnDeath(AController* KillerController)
{
	if (Health <= 0)
	{
		bIsAlive = false;

		OnPlayerDeath.Broadcast(KillerController);

		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			//마우스 커서 표시
			DisableInput(PC);
			PC->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			//사망 UI 위젯 생성 및 표시
			if (DeathWidgetClass)
			{
				TObjectPtr<UDeathWidget> DeathWidgetInstance = CreateWidget<UDeathWidget>(PC, DeathWidgetClass);

				if(DeathWidgetInstance)
				{
					DeathWidgetInstance->OwningController = PC;
					DeathWidgetInstance->AddToViewport();
				}
			}
		}
	}

}

// 피격 함수
float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsAlive) return 0.0f;

	// 1. 방어력 계산 후 실제로 적용할 대미지 계산
	float DamageToApply = FMath::Max(0.0f, DamageAmount - Defence);

	// 2. 체력 감소
	if (DamageToApply > 0)
	{
		// Health는 int32이므로 적용할 대미지를 정수로 변환하여 적용
		Health -= FMath::RoundToInt(DamageToApply);

		// 3. 사망 체크
		if (Health <= 0)
		{
			// **[수정]** OnDeath 호출 시 EventInstigator를 전달
			OnDeath(EventInstigator);
		}
	}

	// 실제로 적용된 대미지를 반환
	return DamageToApply;
}