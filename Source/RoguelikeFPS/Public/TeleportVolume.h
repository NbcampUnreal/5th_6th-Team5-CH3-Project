// TeleportVolume.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportVolume.generated.h"

// 컴포넌트 전방 선언
class UBoxComponent;
class UPrimitiveComponent;
class AController;

UCLASS()
class ROGUELIKEFPS_API ATeleportVolume : public AActor
{
	GENERATED_BODY()

public:
	ATeleportVolume();

protected:
	virtual void BeginPlay() override;

	// 1. 트리거 볼륨 컴포넌트 (액터가 겹치는 영역을 감지)
	// **[수정]** TObjectPtr 적용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleport")
	TObjectPtr<UBoxComponent> OverlapVolume;

	// 2. 목적지 레벨 이름 (에디터에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	FName DestinationLevelName = TEXT("L_GameMap02"); // 기본값 설정 (다음 레벨)

public:
	// 3. 오버랩 이벤트 핸들러 함수 선언
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};