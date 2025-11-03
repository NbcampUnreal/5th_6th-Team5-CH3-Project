#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode() {}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (AFPSCharacter* Player = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        // 이벤트 바인딩
        Player->OnPlayerLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        Player->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
        Player->OnPlayerStageClear.AddDynamic(this, &AFPSGameMode::HandleStageClear);
    }
}

void AFPSGameMode::HandlePlayerLevelUp()
{
    UE_LOG(LogTemp, Log, TEXT("[GameMode] Player Level Up → Augment UI"));
}

void AFPSGameMode::HandlePlayerDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("[GameMode] Player Died → Show Death UI"));
}

void AFPSGameMode::HandleStageClear()
{
    UE_LOG(LogTemp, Log, TEXT("[GameMode] Stage Cleared → Prepare Next Level"));
}

void AFPSGameMode::LoadLevel(FName LevelName)
{
    UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
