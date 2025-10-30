// FPSCharacter.cpp

#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

		// **[수정]** CrouchedHalfHeight 경고 제거를 위해 함수 호출로 변경
		GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
	}

	DashMultifly = 10.0f;
	DashSpeed = MovingSpeed * DashMultifly;
	DashTime = 1.0f;
}

// **[추가]** BeginPlay (입력 매핑 컨텍스트 설정을 제거했으므로 비워둡니다)
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// **[수정]** 전통적인 Input Axis/Action 바인딩 예시 (Blueprint 설정에 따라 변경될 수 있음)
	// Blueprint에서 Input을 처리하려면 이 함수를 비워둡니다.

	// Axis 바인딩 예시:
	// PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	// PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// Action 바인딩 예시:
	// PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPSCharacter::StartJump);
	// PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AFPSCharacter::StopJump);
}


// **[수정]** 전통적인 Input Axis 바인딩 시 float을 받도록 수정
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


// **[수정]** Action 바인딩 시 입력 값 없이 호출되도록 수정
void AFPSCharacter::StartJump()
{
	Jump();
}
// **[수정]** Action 바인딩 시 입력 값 없이 호출되도록 수정
void AFPSCharacter::StopJump()
{
	StopJumping();
}

// **[수정]** Action 바인딩 시 입력 값 없이 호출되도록 수정
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


// **[수정]** Action 바인딩 시 입력 값 없이 호출되도록 수정
void AFPSCharacter::StartCrouch()
{
	Crouch();
}

// **[수정]** Action 바인딩 시 입력 값 없이 호출되도록 수정
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


// 사망 함수
void AFPSCharacter::OnDeath(AController* KillerController)
{
	if (Health <= 0)
	{
		bIsAlive = false;

		// **[추가]** 사망 델리게이트 발송
		OnPlayerDeath.Broadcast(KillerController);

		// ... (사망 처리 로직)
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