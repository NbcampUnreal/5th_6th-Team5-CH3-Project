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

    // 인지/전투 파라미터 (몹별로 다르게 세팅)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float SightRadius = 1800.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float LoseSightRadius = 2200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
    float PeripheralVisionAngle = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float AttackRange = 250.f;
};
