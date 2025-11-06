#pragma once
//JMS
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyConfig.generated.h"

// EnemyConfig.h

UCLASS(BlueprintType)
class UEnemyConfig : public UDataAsset
{
    GENERATED_BODY()
public:

    // 인지/전투 범위
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float SightRadius = 1800.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float LoseSightRadius = 2200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float PeripheralVisionAngle = 90.f;

    //스탯 
    //공, 방, 체, 최대체력, 공속, 이속,
    //이후 추적 시간 추가
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float MaxHP = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float HP = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float HPperLevel = 15.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float ATK = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float ATKperLevel = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float DEF = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float DEFperLevel = 6.67f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
    float AttackRange = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
    float AttackCooldown = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Movement")
    float WalkSpeed = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Movement")
    float ChaseSpeed = 500.f;


    // 정찰 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
    float PatrolRadius = 600.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
    float PatrolWaitMin = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
    float PatrolWaitMax = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
    float AcceptanceRadius = 120.f;


    // 근거리 전용
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Melee")
    float MeleeRange = 220.f;  // 스윕 길이(초록 캡슐 길이)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Melee")
    float MeleeHitRadius = 60.f;   // 스윕 두께(초록 캡슐 반지름)

    // 원거리 전용
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Ranged")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Ranged")
    float ProjectileSpeed = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Ranged")
    FName MuzzleSocket = TEXT("Muzzle");

    // 보스 전용

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Boss")
    float AddtionalRange = 0.f;



};
