
#include "AI/Character/AIEnemyCharacter.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"  
#include "Engine/DamageEvents.h"
#include "AI/Character/AIEnemyController.h"
#include "AI/Structure/Component/MeleeAttackComponent.h"
#include "AI/Structure/Component/RangedAttackComponent.h"
#include "AI/Structure/Component/Stage2BossAttackComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DropItem.h"

#include "GameFramework/DamageType.h"
#include "Components/CapsuleComponent.h"



AAIEnemyCharacter::AAIEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}



void AAIEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!StateMachine)
        StateMachine = FindComponentByClass<UEnemyStateMachineComponent>();

    ensureMsgf(Config, TEXT("Enemy Config is null on %s"), *GetName());


    if (ensure(Config))
    {
        // 스탯 적용
        Level = Config->Level + FMath::RandRange(0, 3); 
        MAXHP = Config->MaxHP + Level * Config->HPperLevel; 
        HP = MAXHP; 
        ATK = Config->ATK + Level * Config->ATKperLevel; 
        DEF = Config->DEF + Level * Config->DEFperLevel;

        UE_LOG(LogTemp, Log, TEXT("Level : %d"), Level);
        UE_LOG(LogTemp, Log, TEXT("HP : %f"), MAXHP);
        UE_LOG(LogTemp, Log, TEXT("ATK : %f"), ATK);
        UE_LOG(LogTemp, Log, TEXT("DEF : %f"), DEF);
        // 이동 속도
        ApplyWalkSpeed();
    }
    ApplyConfigToComponents(ATK);

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

float AAIEnemyCharacter::GetWalkSpeed()
{
    float Speed = (Config ? Config->WalkSpeed : 300.f);
    return Speed;
}

float AAIEnemyCharacter::GetChaseSpeed()
{
    const float Speed = (Config && Config->ChaseSpeed > 0.f) ? Config->ChaseSpeed : (Config ? Config->WalkSpeed : 500.f);
    return Speed;
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

void AAIEnemyCharacter::ApplyConfigToComponents(float Damage)
{
    if (!Config) return;

    if (auto* Melee = FindComponentByClass<UMeleeAttackComponent>())
    {
        Melee->ApplyMeleeConfig(Config, Damage);
        //UE_LOG(LogTemp, Log, TEXT("ApplyMeleeConfig Complete"));
    }


    if (auto* Ranged = FindComponentByClass<URangedAttackComponent>())
    {
        Ranged->ApplyRangeConfig(Config, Damage);
        //UE_LOG(LogTemp, Log, TEXT("ApplyRangeConfig Complete"));
    }

    if (auto* Boss2 = FindComponentByClass<UStage2BossAttackComponent>())
    {
        Boss2->ApplyMeleeConfig(Config, Damage);
        //UE_LOG(LogTemp, Log, TEXT("ApplyMeleeConfig Complete"));
    }



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

void AAIEnemyCharacter::AddHealth(float Amount)
{
    HP = FMath::Clamp(HP + Amount, 0.0f, MAXHP);
    //UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), HP);
}

void AAIEnemyCharacter::OnDeath()
{
    if (StateMachine)
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            AICon->StopMovement();
        }
        if (AAIEnemyController* AICon = Cast<AAIEnemyController>(GetController()))
        {
            AICon->SendDeadToBT();
        }
        //AActor* Proj = GetWorld()->SpawnActor<AActor>(DropItemClass, GetActorLocation(), GetActorRotation());
        DropItem();
        StateMachine->ChangeState(EEnemyState::Dead);
    }
}

void AAIEnemyCharacter::ApplyStun(float Duration)
{
    if (bIsStunned) return;
    bIsStunned = true;

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->DisableMovement();
    }
    if (AAIEnemyController* AICon = Cast<AAIEnemyController>(GetController()))
    {
        AICon->SendStunToBT(true);
    }
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();
    }

    GetWorldTimerManager().SetTimer(StunTimerHandle, [this]()
    {
        bIsStunned = false;
        if (UCharacterMovementComponent* Move = GetCharacterMovement())
        {
            Move->SetMovementMode(MOVE_Walking);
        }
        if (AAIEnemyController* AICon = Cast<AAIEnemyController>(GetController()))
        {
            AICon->SendStunToBT(false);
        }

        UE_LOG(LogTemp, Warning, TEXT("[STUN END] Enemy recovered"));
    }, Duration, false);

    UE_LOG(LogTemp, Warning, TEXT("[STUN START] Enemy stunned for %.1f sec"), Duration);
}

void AAIEnemyCharacter::DropItem()
{
    if (DropTable)
    {
        TArray<FDropTable*> AllRows;
        static const FString ContextString(TEXT("ItemSpawnContext"));
        DropTable->GetAllRows<FDropTable>(ContextString, AllRows);
        if (AllRows.IsEmpty()) return;

        for (const FDropTable* Row : AllRows)
        {
            const float RandValue = FMath::FRandRange(0.0f, 100.f);
            if (RandValue <= Row->SpawnChance)
            {
                FVector randLocation = FVector();

                for (int i = 0; i < Row->ItemAmount; ++i)
                {
                    // 드랍 위치 사망한 캐릭터 주변으로 생성
                    const float randX = FMath::FRandRange(-50.0f, 50.f);
                    const float randY = FMath::FRandRange(-50.0f, 50.f);
                    AActor* Proj = GetWorld()->SpawnActor<AActor>(Row->ItemClass, FVector(GetActorLocation().X + randX, GetActorLocation().Y + randY, GetActorLocation().Z), GetActorRotation());
                    auto* dropclass = Cast<ADropItem>(Proj);
                    if (dropclass)
                    {
                        const float randAmount = FMath::FRandRange(0.f, Row->AddAmount/2);
                        dropclass->SetADDAmount(Row->AddAmount + (int)randAmount);
                    }
                }
                
            }
        }
   
        //if(RandValue <= AllRows->SpawnChance)
    }
}

float AAIEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
    UE_LOG(LogTemp, Log, TEXT("TakeDamage"));
    float FinalDamage = DamageAmount; 


    AActor* ShooterPawn = DamageCauser ? DamageCauser->GetInstigator() : nullptr;
    if (!ShooterPawn && EventInstigator) ShooterPawn = EventInstigator->GetPawn();
    UE_LOG(LogTemp, Log, TEXT("Name %s"), *ShooterPawn->GetName());

    FVector HitLocation = ShooterPawn->GetActorLocation();

    UE_LOG(LogTemp, Log, TEXT("TakeDamage HITLOCATION : %f, %f, %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);

    if (AAIEnemyController* AICon = Cast<AAIEnemyController>(GetController()))
    {
        AICon->OnDamagedBy(
            /*InstigatorActor=*/ShooterPawn ? ShooterPawn : DamageCauser,
            /*Damage=*/FinalDamage,
            /*EventLocation=*/GetActorLocation(),
            /*HitLocation=*/HitLocation
        );
    }


    const float Applied = Super::TakeDamage(FinalDamage, DamageEvent, EventInstigator, DamageCauser); 
    // HP 처리 

    HP = FMath::Clamp(Applied * (100 / (100 + DEF)), 0.f, MAXHP);
    if (HP <= 0.f) 
    { 
        OnDeath(); 
    } 
    return Applied;
}
