// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/MissileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AMissileActor::SetTarget(AActor* target)
{
    if (target)
    {
        _Target = target;
    }
}

void AMissileActor::BeginPlay()
{
    Super::BeginPlay();

    // Ascend: 위로만 올라가게 속도 설정
    _ProjectileMovement->Velocity = FVector::UpVector * _AscendSpeed;
    _ProjectileMovement->bShouldBounce = false;
    _ProjectileMovement->ProjectileGravityScale = 0.f;

    // 1초 뒤에 유도로 전환
    FTimerHandle AscendTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(AscendTimerHandle, this, &AMissileActor::EnableHoming, 1.0f, false);
}

void AMissileActor::EnableHoming()
{
    if (!_Target) return;

    // 타겟의 콤포넌트 (캡슐/루트 등)를 지정
    UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(_Target->GetRootComponent());
    if (!TargetComp) return;

    _ProjectileMovement->bIsHomingProjectile = true;
    _ProjectileMovement->HomingTargetComponent = TargetComp;
    _ProjectileMovement->HomingAccelerationMagnitude = _HomingAccel; // 적절히 튜닝
    _ProjectileMovement->Velocity = (_Target->GetActorLocation() - GetActorLocation()).GetSafeNormal() * _CruiseSpeed;
}
