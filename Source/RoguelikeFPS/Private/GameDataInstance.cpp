#include "GameDataInstance.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "Inventory.h"
#include "Weapon/GunComponent.h"


UGameDataInstance::UGameDataInstance()
{
    // ===== 기본값 설정 (생성자에서만) =====
    SelectedWeaponIndex = 0;

    // 스테이지 초기화
    CurrentStageIndex = 0;

    // 기본 맵 목록 (에디터에서 수정 가능)
    StageLevelNames.Empty();
    StageLevelNames.Add(FName("Level1"));
    StageLevelNames.Add(FName("Level1"));
    StageLevelNames.Add(FName("Level1"));

    MainMenuLevelName = FName("Level1");
}

void UGameDataInstance::SetSelectedOption(int32 WeaponIndex)
{
    SelectedWeaponIndex = WeaponIndex;
}

int32 UGameDataInstance::GetSelectedOption() const
{
    return SelectedWeaponIndex;
}

void UGameDataInstance::ResetGameStatsToLevelOne()
{
    SelectedWeaponIndex = 1;
    bIsReadyToStart = false;
    CurrentStageIndex = 1;  //게임 오버/클리어 후 재시작을 위해 CurrentStageIndex 리셋
}

void UGameDataInstance::LoadStatus(AFPSCharacter* Character)
{
    //캐릭터 스탯 로드
    Character->SetLevel(level);
    Character->SetHealth(health);
    Character->SetMaxHealth(maxhealth);
    Character->SetAttack(attack);
    Character->SetDefence(defence);
    Character->SetAttackSpeed(AttackSpeed);
    Character->SetMovingSpeed(MovingSpeed);
    Character->SetStamina(Stamina);
    Character->SetExperience(Experience);
    Character->SetMaxExperience(MaxExperience);


    UE_LOG(LogTemp, Warning, TEXT("Load Level = %d"), level);

    if (SelectedWeaponIndex == 1)
    {
        AActor* Proj = GetWorld()->SpawnActor<AActor>(PistolBP, FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z - 150.f), Character->GetActorRotation());
    }
    if (SelectedWeaponIndex == 2)
    {
        AActor* Proj = GetWorld()->SpawnActor<AActor>(RifleBP, FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z - 150.f), Character->GetActorRotation());
    }
    if (SelectedWeaponIndex == 3)
    {
        AActor* Proj = GetWorld()->SpawnActor<AActor>(ShotGunBP, FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z - 150.f), Character->GetActorRotation());
    }
    if (SelectedWeaponIndex == 4)
    {
        AActor* Proj = GetWorld()->SpawnActor<AActor>(RailGunBP, FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z - 150.f), Character->GetActorRotation());
    }
    
    //인벤토리 데이터 로드
    //if (invencomp)
    //{
    //    Character->Inventory = Cast<UInventory>(invencomp);
    //}

}

void UGameDataInstance::SaveStatus(AFPSCharacter* Character)
{
    //캐릭터 스탯 저장
    level = Character->GetLevel();
    health = Character->GetHealth();
    maxhealth = Character->GetMaxHealth();
    attack = Character->GetAttack();
    defence = Character->GetDefence();
    AttackSpeed = Character->GetAttackSpeed();
    MovingSpeed = Character->GetMovingSpeed();
    Stamina = Character->GetStamina();
    Experience = Character->GetExperience();
    MaxExperience = Character->GetMaxExperience();

    UE_LOG(LogTemp, Warning, TEXT("Save Level = %d"), level);

    //총 컴포넌트 데이터 저장
    //guncomp = Cast<UGunComponent>(Character->GetComponentByClass<UGunComponent>());

    //인벤토리 데이터 저장
    //invencomp = Cast<UInventory>(Character->GetComponentByClass<UInventory>());
}


UGameDataInstance* UGameDataInstance::GetGameDataInstance(const UObject* WorldContext)
{
    if (!WorldContext || !WorldContext->GetWorld())
    {
        return nullptr;
    }

    return Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(WorldContext));
}

void UGameDataInstance::InitializeGameData()
{
    // 에디터에서 설정 안 됐을 경우만 기본값 적용
    if (StageLevelNames.Num() == 0)
    {
        StageLevelNames.Add(TEXT("L_GameMap1"));
        StageLevelNames.Add(TEXT("L_GameMap2"));
        StageLevelNames.Add(TEXT("L_GameMap3"));
    }

    if (MainMenuLevelName.IsNone())
    {
        MainMenuLevelName = TEXT("L_MainMenu");
    }

    CurrentStageIndex = 0;

    UE_LOG(LogTemp, Log, TEXT("GameDataInstance Initialized!"));
}