#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "FPSPlayerController.h"
#include "Engine/Engine.h"

AFPSCharacter::AFPSCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ===== 카메라 세팅 =====
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 50.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);
    CameraComp->bUsePawnControlRotation = false;

    // ===== 이동 =====
    GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->CrouchedHalfHeight = 60.0f;

    // ===== 대시 =====
    DashSpeed = MovingSpeed * DashMultiplier;
}

void AFPSCharacter::BeginPlay()
{
    Super::BeginPlay();
    InitializeWeapon(TEXT("Pistol"));
}

void AFPSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 실제 Input 바인딩은 블루프린트에서 설정 가능
    }
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();
    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), Input.Y);
        AddMovementInput(GetActorRightVector(), Input.X);
    }
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AFPSCharacter::StartJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>()) Jump();
}

void AFPSCharacter::StopJump(const FInputActionValue& Value)
{
    StopJumping();
}

void AFPSCharacter::StartDash(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
    GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AFPSCharacter::StopDash, DashTime, false);
}

void AFPSCharacter::StopDash()
{
    GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
}

// ===== 전투 관련 =====
void AFPSCharacter::FireWeapon()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("FireWeapon() Triggered"));
}

// ===== 스탯 관련 =====
void AFPSCharacter::AddXP(float Amount)
{
    CurrentXP += Amount;
    if (CurrentXP >= MaxXP)
    {
        LevelUp();
    }
}

void AFPSCharacter::LevelUp()
{
    PlayerLevel++;
    Attack += 3;
    MaxHealth += 20;
    Health = MaxHealth;
    CurrentXP = 0;

    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("LEVEL UP!"));
    OnPlayerLevelUp.Broadcast();
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = FMath::Max(0.f, DamageAmount - Defence);
    Health -= ActualDamage;

    if (Health <= 0 && bIsAlive)
    {
        HandleDeath();
    }

    return ActualDamage;
}

void AFPSCharacter::HandleDeath()
{
    bIsAlive = false;
    OnPlayerDeath.Broadcast();
}

void AFPSCharacter::HandleStageClear()
{
    OnPlayerStageClear.Broadcast();
}

void AFPSCharacter::ApplyAugment(FName AugmentID)
{
    if (AugmentID == TEXT("Stronger"))
    {
        Attack += 5;
    }
    else if (AugmentID == TEXT("Faster"))
    {
        MovingSpeed += 50;
        GetCharacterMovement()->MaxWalkSpeed = MovingSpeed;
    }
}

void AFPSCharacter::InitializeWeapon(FName SelectedWeaponName)
{
    if (PistolMesh) PistolMesh->SetVisibility(false);
    if (RifleMesh) RifleMesh->SetVisibility(false);
    if (ShotgunMesh) ShotgunMesh->SetVisibility(false);

    if (SelectedWeaponName == TEXT("Pistol") && PistolMesh)
        PistolMesh->SetVisibility(true);
    else if (SelectedWeaponName == TEXT("Rifle") && RifleMesh)
        RifleMesh->SetVisibility(true);
    else if (SelectedWeaponName == TEXT("Shotgun") && ShotgunMesh)
        ShotgunMesh->SetVisibility(true);
}
