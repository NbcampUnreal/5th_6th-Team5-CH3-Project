#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportVolume.generated.h"

UCLASS()
class ROGUELIKEFPS_API ATeleportVolume : public AActor
{
    GENERATED_BODY()

public:
    ATeleportVolume();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    class UBoxComponent* TeleportTrigger;

    UPROPERTY(EditAnywhere, Category = "Teleport")
    float DelayBeforeTeleport = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Teleport", meta = (ToolTip = "다음으로 이동할 맵의 레벨 이름"))
    FName NextLevelName = TEXT("NextMap");

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
        class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

private:
    // 타이머에서 참조할 대상 저장
    TWeakObjectPtr<AActor> PendingTeleportActor;

    // 타이머 핸들러
    FTimerHandle TeleportTimerHandle;

    // 타이머에서 호출할 함수
    void TeleportPendingActor();
};
