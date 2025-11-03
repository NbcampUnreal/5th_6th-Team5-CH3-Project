#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillFeedComponent.generated.h"

class AFPSCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UKillFeedComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UKillFeedComponent();

protected:
    UPROPERTY(EditAnywhere, Category = "XP")
    float XPAmountOnDeath = 50.0f; // 이 적을 잡았을 때 얻는 XP

public:
    // 적이 죽었을 때 호출될 함수 (BP에서도 호출 가능)
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ReportDeathToKiller(AActor* KillerActor);
};