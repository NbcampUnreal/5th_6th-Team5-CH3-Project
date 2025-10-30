#include "TeleportVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h" // ACharacter 사용을 위해 필요
#include "Kismet/GameplayStatics.h" // 레벨 전환을 위해 필요
#include "Kismet/KismetSystemLibrary.h" // 디버그 로그를 위해 필요

ATeleportVolume::ATeleportVolume()
{
    PrimaryActorTick.bCanEverTick = false; // 매 프레임 틱 불필요

    // 1. BoxComponent 생성 및 루트 설정
    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
    RootComponent = OverlapVolume;

    // 2. 오버랩 이벤트 바인딩 설정
    // 이 BoxComponent의 Begin Overlap 이벤트를 OnOverlapBegin 함수와 연결
    OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ATeleportVolume::OnOverlapBegin);
}

void ATeleportVolume::BeginPlay()
{
    Super::BeginPlay();
}

void ATeleportVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 1. 오버랩한 액터가 플레이어인지 확인 (캐릭터 클래스 확인)
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (!PlayerCharacter) return; // 플레이어 캐릭터가 아니면 리턴

    // 2. 플레이어 캐릭터가 맞다면 레벨 전환 로직 실행
    if (!DestinationLevelName.IsNone())
    {
        // 디버그 로그
        UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Teleporting Player to: %s"), *DestinationLevelName.ToString()), true, true, FColor::Cyan, 5.0f);

        // 레벨 전환 실행
        // ClientTravel을 사용하여 멀티플레이 환경에서도 안전한 전환
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->ClientTravel(DestinationLevelName.ToString(), ETravelType::TRAVEL_Absolute);
        }
    }
}
