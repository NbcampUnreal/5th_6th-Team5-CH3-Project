// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 선택된 무기 옵션의 인덱스 (0: 선택 없음, 1~4: 무기 번호)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Data")
	int32 SelectedWeaponIndex = 0;

	// 게임 시작 가능 상태 (무기 선택 여부)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Data")
	bool bIsReadyToStart = false;

public:
	// 선택 인덱스를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void SetSelectedOption(int32 OptionIndex);
};