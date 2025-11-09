#include "TitleWidget.h"
#include "TeleportVolume.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

bool UTitleWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);

    if (OptionButton)
        OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);

    return true;
}

void UTitleWidget::NativeConstruct()
{
    //Super::NativeConstruct();

    //// 혹시 모를 커서 활성화
    //APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //if (PC)
    //{
    //    PC->bShowMouseCursor = true;
    //    PC->SetInputMode(FInputModeUIOnly());
    //}
}

void UTitleWidget::OnGameStartClicked()
{
    if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
    {
        UIManager->ShowWeaponSelectMenu();
    }
}

void UTitleWidget::OnOptionClicked()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu Placeholder"));
}

void UTitleWidget::OnExitClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}

//void ATeleportVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
//    bool bFromSweep, const FHitResult& SweepResult)
//{
//    if (!OtherActor) return;
//    if (OtherActor->IsA<AFPSCharacter>())
//    {
//        if (GetWorldTimerManager().IsTimerActive(TeleportTimerHandle))
//        {
//            GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
//        }
//        PendingTeleportActor = OtherActor;
//        GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ATeleportVolume::TeleportPendingActor, DelayBeforeTeleport, false);
//        if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
//        {
//            if (UUserWidget* Widget = UIManager->ShowWidget(UIManager->TeleportCountdownWidgetClass, EUILayer::Popup))
//            {
//                if (UTeleportCountdownWidget* CountdownWidget = Cast<UTeleportCountdownWidget>(Widget))
//                {
//                    CountdownWidget->StartCountdown(DelayBeforeTeleport);
//                }
//            }
//        }
//        if (GEngine)
//        {
//            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("텔레포트 카운트다운 시작"));
//        }
//    }
//}