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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    float AttackPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    int32 CurrentBulletCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    int32 MaxBulletCount; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    float AttackDelay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    float ReloadTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    float ProjectileSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    int ProjectilesPerShot = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    float Accuracy = 1.0f;

    // 생성자에서 기본값 초기화도 가능
    /*FGunStatus()
        : Damage(10.f)
        , Offset(FVector::ZeroVector)
    {
    }*/
};
