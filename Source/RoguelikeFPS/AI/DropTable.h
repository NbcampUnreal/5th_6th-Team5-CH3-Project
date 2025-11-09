// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropTable.generated.h"

USTRUCT(BlueprintType)
struct FDropTable : public FTableRowBase
{
	GENERATED_BODY()
	
public:
    // 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;
    // 어떤 아이템 클래스를 스폰할지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemClass;
    // 이 아이템의 스폰 확률
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;
    // 추가할 수치
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AddAmount;
    // 이 아이템의 스폰 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemAmount;

};
