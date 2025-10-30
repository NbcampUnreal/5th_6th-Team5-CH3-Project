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

	// **[추가]** Level 1의 기본 스탯 저장 (캐릭터 초기화에 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Stats")
	int32 BaseMaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Stats")
	int32 BaseAttack = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Stats")
	int32 BaseDefence = 10;

public:
	// 선택 인덱스를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void SetSelectedOption(int32 OptionIndex);

	// **[추가]** 게임 재시작을 위한 기본 스탯 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void ResetGameStatsToLevelOne();
};
