#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "FPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatsComponent.h" 
#include "Kismet/GameplayStatics.h" 


AFPSCharacter::AFPSCharacter()
// 기존 스탯 변수 초기화 코드 주석 처리 (UStatsComponent로 관리)
// : Level(1),
// Health(100),
// MaxHealth(100),
// Attack(10),
// Defence(10),
// AttackSpeed(5),
// MovingSpeed(600),
// Stamina(500),
// Experience(0),
// MaxExperience(100),
// bIsAlive(true)
{
	PrimaryActorTick.bCanEverTick = false;

	// UStatsComponent 부착 추가
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

	// 카메라 부착
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 50.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// 기존 MovingSpeed 대신 StatsComp의 MovementSpeed를 사용하도록 수정
	// GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
	// MaxWalkSpeed 설정은 UStatsComponent::BeginPlay에서 처리할 예정 

	DashMultifly = 10.0f;
	// MovingSpeed 대신 하드코딩된 값 사용 또는 StatsComp의 값 사용 (일단 주석 처리)
	// DashSpeed = MovingSpeed * DashMultifly;
	DashTime = 1.0f;

	// Crouch 활성화
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	// CrouchHalfHeight는 UCharacterMovementComponent에서 권장되지 않는 속성
	GetCharacterMovement()->CrouchedHalfHeight = 60.0f;
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPSCharacter::StartDash(const FInputActionValue& value)
{
	// DashSpeed는 UStatsComponent의 MovementSpeed를 기반으로 계산되어야 합니다.
	float DashSpeed = StatsComp ? StatsComp->CurrentStats.MovementSpeed * DashMultifly : 600.0f * DashMultifly;

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
	// MovingSpeed 대신 UStatsComponent의 MovementSpeed를 사용하도록 수정
	float BaseSpeed = StatsComp ? StatsComp->CurrentStats.MovementSpeed : 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

// ... (StartCrouch, StopCrouch 함수 내용 유지) ...


// 레벨업 함수: UStatsComponent로 이동했으므로 삭제
/*
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
*/


// 사망 함수: Die 함수로 대체하고, 기존 OnDeath 로직을 통합
void AFPSCharacter::Die(AController* KillerController)
{
	// bIsAlive가 있다면 체크 (없다면 AFPSCharacter.h에서 제거해야 함)
	// if (!bIsAlive) return; 
	// bIsAlive = false; 

	// 사망 델리게이트 발송 (GameMode에게 알림)
	OnPlayerDeath.Broadcast(KillerController);

	// 캐릭터 사망 처리 (입력 비활성화)
	DisableInput(GetController<APlayerController>());

	// ... (추가적인 사망 애니메이션, Ragdoll 처리 등)
}

// 기존 OnDeath 함수 삭제
/*
void AFPSCharacter::OnDeath()
{
	if (Health <= 0)
	{
		bIsAlive = false;
		// 사망 시 게임 오버 레벨 호출
	}
}
*/


// 피격 함수: UStatsComponent의 스탯을 사용하도록 수정
float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, TObjectPtr<AController> EventInstigator, TObjectPtr<AActor> DamageCauser)
{
	// UStatsComponent가 유효한지 확인
	if (!StatsComp)
	{
		// StatsComp가 없다면 기본 로직을 수행하거나 대미지를 무시
		return DamageAmount;
	}

	// 방어력(Defence)을 StatsComp의 스탯 구조체에서 가져와 사용
	// TObjectPtr을 AController*로 안전하게 변환
	AController* InstigatorPtr = EventInstigator.Get();

	// UStatsComponent의 스탯 값을 사용한다고 가정하고, 기존 Health 대신 MaxHealth를 방어력으로 사용 (임시)
	// float CharacterDefence = StatsComp->GetCurrentStats().MaxHealth; // 예시: MaxHealth는 방어력이 아님

	// StatsComp에 방어력(Defence) 변수가 없으므로, Attack 변수를 방어력으로 사용한다고 가정 (임시)
	// float CharacterDefence = StatsComp->CurrentStats.Attack; // 이 코드는 StatsComp에 Defence가 있다면 사용

	// 현재 Health 변수를 AFPSCharacter.h에 그대로 유지하고 Health를 여기서 업데이트한다고 가정합니다.
	float ActualDamage = DamageAmount - Defence; // 기존 Defence 변수를 그대로 사용

	if (ActualDamage > 0)
	{
		// 체력 감소: 기존 Health 변수 사용
		Health -= FMath::RoundToInt(ActualDamage); // float에서 int로 변환하여 감소

		if (Health <= 0)
		{
			Die(InstigatorPtr); // 사망 시 EventInstigator를 KillerController로 전달
		}
	}

	return ActualDamage;
}