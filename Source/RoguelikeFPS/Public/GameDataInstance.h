#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

class AFPSCharacter;
class UGunComponent;
class UInventory;
class UGunDataAsset;

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UGameDataInstance();

    // ===== 플레이어 선택/상태 데이터 =====
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 SelectedWeaponIndex;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    bool bIsReadyToStart;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 level = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 health = 100;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 maxhealth = 100;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 attack = 10;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 defence = 10;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 AttackSpeed = 5;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 MovingSpeed = 600;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 Stamina = 500;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float Experience = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float MaxExperience = 100;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    UGunComponent* guncomp;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    UInventory* invencomp;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    UGunComponent* initpistolcomp;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    UGunComponent* initriflecomp;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    TSubclassOf<AActor> PistolBP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    TSubclassOf<AActor> RifleBP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    TObjectPtr<UGunDataAsset> GunData;


    UFUNCTION()
    void RoadStatus(AFPSCharacter* Character);

    UFUNCTION()
    void SaveStatus(AFPSCharacter* Character);


    // 정적 getter (C++ & 블루프린트)
    UFUNCTION(BlueprintCallable, Category = "GameData")
    static UGameDataInstance* GetGameDataInstance(const UObject* WorldContext);

    UFUNCTION(BlueprintCallable, Category = "GameData")
    void InitializeGameData();

    // 스테이지 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    TArray<FName> StageLevelNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    FName MainMenuLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    int32 CurrentStageIndex = 0;

public:
    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void SetSelectedOption(int32 WeaponIndex);

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    int32 GetSelectedOption() const;

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void ResetGameStatsToLevelOne();

};