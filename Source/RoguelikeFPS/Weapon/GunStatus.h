// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunStatus.generated.h"
/**
 * 
 */
class ROGUELIKEFPS_API GunStatus
{
public:
	GunStatus();
	~GunStatus();
};

USTRUCT(Atomic, BlueprintType)
struct FGunStatus
{
    GENERATED_BODY()

    // 편집·블루프린트 노출
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float AttackPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    int32 MaxBulletCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float AttackDelay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float ReloadTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float ProjectileSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    int ProjectilesPerShot = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float Accuracy = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float Spread_Angle = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float CriticalChance = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float CriticalMultiplier = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float HeadShotMultiplier = 5.f;


    // 생성자에서 기본값 초기화도 가능
    /*FGunStatus()
        : Damage(10.f)
        , Offset(FVector::ZeroVector)
    {
    }*/
};
