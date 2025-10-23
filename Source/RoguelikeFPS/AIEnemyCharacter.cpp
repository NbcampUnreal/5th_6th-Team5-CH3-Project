#include "AIEnemyCharacter.h"
#include "AIEnemyController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIEnemyCharacter::AAIEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAIEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    ensureMsgf(Config, TEXT("Enemy Config is null on %s"), *GetName());

    // 기본 이동속도 적용
    ApplyWalkSpeed();
}

void AAIEnemyCharacter::ApplyWalkSpeed()
{
    float Speed = (Config ? Config->WalkSpeed : 300.f);
    GetCharacterMovement()->MaxWalkSpeed = Speed;
    CurrentMoveSpeed = Speed;
}

void AAIEnemyCharacter::ApplyChaseSpeed()
{
    // Config에 ChaseSpeed가 없으면 WalkSpeed로 대체
    const float Speed = (Config && Config->ChaseSpeed > 0.f) ? Config->ChaseSpeed : (Config ? Config->WalkSpeed : 500.f);
    GetCharacterMovement()->MaxWalkSpeed = Speed;
    CurrentMoveSpeed = Speed;
}

void AAIEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

#if !(UE_BUILD_SHIPPING)   // 에디터/개발에서만
    if (bDrawPerceptionDebug)
    {
        DrawPerceptionGizmos();
    }
#endif
}

void AAIEnemyCharacter::DrawPerceptionGizmos()
{
    AAIEnemyController* AICon = Cast<AAIEnemyController>(GetController());
    if (!AICon) return;

    // 컨트롤러에 설정된 Perception 값들 읽기
    const float SightRadius = AICon->SightRadius;
    const float LoseSightRadius = AICon->LoseSightRadius;
    const float FOVHalfAngleDeg = AICon->PeripheralVisionAngle; // 좌우 반각(°)

    const FVector Origin = GetActorLocation();
    const FVector Forward = GetActorForwardVector();
    const FVector Up = FVector::UpVector;
    const FVector Right = GetActorRightVector();

    UWorld* World = GetWorld();
    if (!World) return;

    // 1) 시야 반경 원 (연두색)
    DrawDebugCircle(
        World,
        Origin,
        SightRadius,
        DebugSegments,
        FColor::Green,
        false,        // 지속X(프레임마다 갱신)
        0.f,
        0,
        DebugThickness,
        FVector(1, 0, 0), FVector(0, 1, 0),  // XY 평면
        false
    );

    // 2) LoseSight 반경 원 (하늘색)
    DrawDebugCircle(
        World,
        Origin,
        LoseSightRadius,
        DebugSegments,
        FColor::Cyan,
        false,
        0.f,
        0,
        DebugThickness,
        FVector(1, 0, 0), FVector(0, 1, 0),
        false
    );

    // 3) 시야각(FOV) 콘(부채꼴) — 정면 방향으로 시야 “부피” 표시
    // DrawDebugCone(월드, 시작점, 방향, 길이, 가로 반각(rad), 세로 반각(rad), 세그먼트, 색)
    const float HalfAngleRad = FMath::DegreesToRadians(FOVHalfAngleDeg);
    DrawDebugCone(
        World,
        Origin + FVector(0, 0, 50.f),    // 약간 위로 올려서 지면과 겹침 방지(선택)
        Forward,
        SightRadius,
        HalfAngleRad,       // 가로 반각
        HalfAngleRad,       // 세로 반각 (동일하게)
        DebugSegments,
        FColor::Green,
        false,
        0.f,
        0,
        DebugThickness
    );
}
