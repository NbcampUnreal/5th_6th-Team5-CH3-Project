#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // DefaultPawnClass는 FPSCharacter로
    DefaultPawnClass = AFPSCharacter::StaticClass();
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    //Super::PostLogin(NewPlayer);

    //if (!NewPlayer || !NewPlayer->GetPawn()) return;

    //// 레벨업 이벤트 구독
    //if (UStatsComponent* StatsComp = NewPlayer->GetPawn()->FindComponentByClass<UStatsComponent>())
    //{
    //    StatsComp->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
    //}

    //// 사망 이벤트 구독
    //if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
    //{
    //    FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
    //}
}

void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController || !AugmentWidgetClass) return;

    PlayerController->SetPause(true);
    FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    UAugmentWidget* AugmentWidget = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);
    if (AugmentWidget)
    {
        //AugmentWidget->OwningController = PlayerController;
        //AugmentWidget->TargetStatsComponent = PlayerController->GetPawn()->FindComponentByClass<UStatsComponent>();
        AugmentWidget->AddToViewport();
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Augment Selection Screen Opened."), true, true, FColor::Orange, 10.f);
    }
}

void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    APlayerController* DeadPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
    if (!DeadPlayerController) return;

    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GAME OVER! Returning to Main Menu."), true, true, FColor::Red, 10.f);

    DeadPlayerController->SetPause(true);
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_MainMenu"));
}
