//#include "FPSCharacter.h"
//#include "Components/SkeletalMeshComponent.h"
//#include "Components/InputComponent.h"
//#include "Kismet/GameplayStatics.h"
//#include "EnhancedInputComponent.h"
//#include "FPSPlayerController.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/SpringArmComponent.h" //  추가 
//#include "Camera/CameraComponent.h" //  추가 
//
//AFPSCharacter::AFPSCharacter()
////  모든 스탯 직접 초기화 
//	: Level(1),
//	Health(100),
//	MaxHealth(100),
//	Attack(10),
//	Defence(10),
//	AttackSpeed(5),
//	MovingSpeed(600),
//	Stamina(500),
//	Experience(0),
//	MaxExperience(100),
//	bIsAlive(true),
//	DashMultifly(10.0f),
//	DashTime(1.0f)
//{
//	PrimaryActorTick.bCanEverTick = true;
//
//	// StatsComponent 생성 제거
//	// StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent")); //  제거됨 
//
//	// 카메라 부착
//	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
//	SpringArmComp->SetupAttachment(RootComponent);
//	SpringArmComp->TargetArmLength = 50.0f;
//	SpringArmComp->bUsePawnControlRotation = true;
//
//	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
//	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
//	CameraComp->bUsePawnControlRotation = false;
//
//	// 대시 스피드 및 Crouch 설정
//	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
//	DashSpeed = MovingSpeed * DashMultifly;
//	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
//	GetCharacterMovement()->CrouchedHalfHeight = 60.0f;
//}
//
//
//void AFPSCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	//  StatsComponent 이벤트 구독 코드 제거 
//	InitializeWeapon(TEXT("Pistol"));
//}
//
//void AFPSCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}
//
//void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	// ... (Enhanced Input 바인딩 코드 유지) ...
//	if (TObjectPtr<UEnhancedInputComponent> EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		if (TObjectPtr<AFPSPlayerController> PlayerController = Cast<AFPSPlayerController>(GetController()))
//		{
//			// Move, Look, Jump, Crouch 바인딩 유지
//			// ...
//
//			//  FireAction 바인딩은 Controller에 멤버가 없으므로 제거된 상태로 유지됩니다. 
//			// 만약 FireAction을 추가하고 싶다면 AFPSPlayerController.h를 수정해야 합니다.
//		}
//	}
//}
//
//void AFPSCharacter::Move(const FInputActionValue& value)
//{
//	if (!Controller) return;
//
//	FVector2D MoveInput = value.Get<FVector2D>();
//
//	if (!FMath::IsNearlyZero(MoveInput.X))
//	{
//		AddMovementInput(GetActorForwardVector(), MoveInput.X);
//	}
//
//	if (!FMath::IsNearlyZero(MoveInput.Y))
//	{
//		AddMovementInput(GetActorRightVector(), MoveInput.Y);
//	}
//}
//void AFPSCharacter::Look(const FInputActionValue& value)
//{
//	FVector2D LookInput = value.Get<FVector2D>();
//
//	AddControllerYawInput(LookInput.X);
//	AddControllerPitchInput(LookInput.Y);
//}
//void AFPSCharacter::StartJump(const FInputActionValue& value)
//{
//	if (value.Get<bool>()) Jump();
//}
//void AFPSCharacter::StopJump(const FInputActionValue& value)
//{
//	if (!value.Get<bool>()) StopJumping();
//}
//void AFPSCharacter::StartDash(const FInputActionValue& value)
//{
//	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
//
//	if (GetWorldTimerManager().IsTimerActive(DashTimerHandle))
//	{
//		GetWorldTimerManager().ClearTimer(DashTimerHandle);
//	}
//
//	GetWorldTimerManager().SetTimer(
//		DashTimerHandle,
//		this,
//		&AFPSCharacter::StopDash,
//		DashTime,
//		false
//	);
//}
//
//void AFPSCharacter::StopDash()
//{
//	GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
//}
//
//void AFPSCharacter::StartCrouch(const FInputActionValue& value)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Crouch Pressed"));
//	Crouch();
//}
//
//void AFPSCharacter::StopCrouch(const FInputActionValue& value)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Crouch Stop"));
//	UnCrouch();
//}
//
//void AFPSCharacter::LevelUp()
//{
//	if (Experience >= MaxExperience) //  MaxExperience 직접 사용 
//	{
//		Level += 1;
//		Health += 20;
//		Attack += 3;
//		Defence += 3;
//		Experience = 0;
//	}
//}
//
//
//void AFPSCharacter::OnDeath()
//{
//	if (Health <= 0)
//	{
//		bIsAlive = false;
//	}
//}
//
//float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, TObjectPtr<AController> EventInstigator, TObjectPtr<AActor> DamageCauser)
//{
//	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) - Defence;
//
//	if (ActualDamage > 0)
//	{
//		Health -= ActualDamage;
//		if (Health <= 0)
//		{
//			OnDeath();
//		}
//	}
//	return ActualDamage;
//}
//
//void AFPSCharacter::FireWeapon()
//{
//	// (비어 있음)
//}
//
//void AFPSCharacter::InitializeWeapon(FName SelectedWeaponName)
//{
//	// ... (기존 코드 유지) ...
//	PistolMesh->SetVisibility(false);
//	RifleMesh->SetVisibility(false);
//	ShotgunMesh->SetVisibility(false);
//
//	if (SelectedWeaponName == TEXT("Pistol"))
//	{
//		PistolMesh->SetVisibility(true);
//	}
//	else if (SelectedWeaponName == TEXT("Rifle"))
//	{
//		RifleMesh->SetVisibility(true);
//	}
//	else if (SelectedWeaponName == TEXT("Shotgun"))
//	{
//		ShotgunMesh->SetVisibility(true);
//	}
//}
//
////  UStatsComponent에서 통합된 함수 구현 
//void AFPSCharacter::LevelUpWithOptions()
//{
//	// 레벨업 로직 호출 (LevelUp 함수 내부 로직을 여기에 통합할 수도 있습니다)
//	LevelUp();
//}
//
//void AFPSCharacter::HandleDeath()
//{
//	OnDeath(); // 캐릭터의 OnDeath 함수를 호출하여 상태를 변경
//}
//
//void AFPSCharacter::HandleStageClear()
//{
//	// 스테이지 클리어 시 필요한 로직 (예: 다음 맵 준비)
//}
//
//void AFPSCharacter::AddXP(float AmountToAdd)
//{
//	CurrentXP += AmountToAdd;
//	// 레벨업 로직 호출
//	LevelUp();
//}
//
//void AFPSCharacter::ApplyAugment(FName AugmentID)
//{
//	// 증강 적용 로직 구현 (예: Attack 스탯 증가 등)
//	if (AugmentID == TEXT("Stronger"))
//	{
//		Attack += 5;
//	}
//	// 레벨업에 따라 증강을 선택하는 로직이 필요할 수 있습니다.
//}