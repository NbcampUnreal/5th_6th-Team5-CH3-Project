#include "FPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h" 
#include "StatsComponent.h" 
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.h" 
#include "GameFramework/Controller.h"

// 헤드샷 상수 정의 (캐릭터 스켈레톤의 Head Bone 이름에 맞춰 수정 필요)
static const float HEADSHOT_MULTIPLIER = 1.5f;
static const FName HEAD_BONE_NAME = TEXT("head");


AFPSCharacter::AFPSCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 50.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
        GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
        GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);
    }
}

void AFPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (StatsComp && GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetMovementSpeed();
    }

#if WITH_EDITOR
    if (!DeathWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DeathWidgetClass is not set on %s"), *GetName());
    }
#endif
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFPSCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookYaw"), this, &AFPSCharacter::LookYaw);
    PlayerInputComponent->BindAxis(TEXT("LookPitch"), this, &AFPSCharacter::LookPitch);

    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFPSCharacter::StartJump);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AFPSCharacter::StopJump);

    PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AFPSCharacter::StartDash);

    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AFPSCharacter::StartCrouch);
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AFPSCharacter::StopCrouch);
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
    if (!bIsAlive || !GetCharacterMovement() || !StatsComp) return;

    // DashMultifly, DashTimerHandle, DashTime은 .h에 선언되어 있어야 합니다.
    float DashSpeed = StatsComp->GetMovementSpeed() * DashMultifly;
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
    if (!bIsAlive || !GetCharacterMovement() || !StatsComp) return;

    GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetMovementSpeed();
}


void AFPSCharacter::StartCrouch()
{
    Crouch();
}

void AFPSCharacter::StopCrouch()
{
    UnCrouch();
}


void AFPSCharacter::OnDeath(AController* KillerController)
{
    if (!bIsAlive) return;

    bIsAlive = false;

    OnPlayerDeath.Broadcast(KillerController);

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        DisableInput(PC);

        if (DeathWidgetClass)
        {
            if (TObjectPtr<UDeathWidget> DeathWidgetInstance = CreateWidget<UDeathWidget>(PC, DeathWidgetClass))
            {
                DeathWidgetInstance->OwningController = PC;
                DeathWidgetInstance->AddToViewport();

                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
    }
}


float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!bIsAlive || !StatsComp) return 0.0f;

    float FinalDamageAmount = DamageAmount;

    // === 1. 헤드샷 판정 (UE5 공식 방식) ===
    if (DamageAmount > 0.0f && DamageEvent.IsOfType(FPointDamageEvent::ClassID))
    {
        const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
        if (PointEvent && PointEvent->HitInfo.BoneName == HEAD_BONE_NAME)
        {
            FinalDamageAmount *= HEADSHOT_MULTIPLIER;

#if WITH_EDITOR
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HEADSHOT!"));
            }
#endif
        }
    }

    // === 2. 대미지 적용 ===
    float DamageTaken = StatsComp->ApplyDamage(FinalDamageAmount);

    // === 3. 사망 체크 ===
    if (StatsComp->GetCurrentHealth() <= 0.0f)
    {
        OnDeath(EventInstigator);
    }

    return DamageTaken;
}