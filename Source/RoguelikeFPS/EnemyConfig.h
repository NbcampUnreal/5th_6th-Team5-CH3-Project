// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyConfig.generated.h"

// EnemyConfig.h
UENUM(BlueprintType)
enum class EEnemyClass : uint8 { Melee, Ranged };

UCLASS(BlueprintType)
class UEnemyConfig : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EEnemyClass EnemyClass = EEnemyClass::Melee;

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
    float MaxHP = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float HP = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float ATK = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float DEF = 0.f;

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
};
