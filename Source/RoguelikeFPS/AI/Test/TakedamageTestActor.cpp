// Fill out your copyright notice in the Description page of Project Settings.


#include "TakedamageTestActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATakedamageTestActor::ATakedamageTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성 및 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 충돌 컴포넌트 생성 및 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// 겹침만 감지하는 프로파일 설정
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	// 루트 컴포넌트로 설정
	Collision->SetupAttachment(Scene);

	// 스태틱 메시 컴포넌트 생성 및 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

	 // Overlap 이벤트 바인딩
	//Collision->OnComponentBeginOverlap.AddDynamic(this, &ATakedamageTestActor::OnItemOverlap);
	//Collision->OnComponentEndOverlap.AddDynamic(this, &ATakedamageTestActor::OnItemEndOverlap);

}

void ATakedamageTestActor::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(Collision))
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ATakedamageTestActor::OnItemOverlap);
		Collision->OnComponentEndOverlap.AddDynamic(this, &ATakedamageTestActor::OnItemEndOverlap);
	}
}


void ATakedamageTestActor::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// OtherActor가 플레이어인지 확인 ("Player" 태그 활용)
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		// 아이템 사용 (획득) 로직 호출
		UGameplayStatics::ApplyDamage(
			OtherActor,                      // 데미지를 받을 액터
			200,            // 데미지 양
			nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
			this,                       // 데미지를 유발한 오브젝트(지뢰)
			UDamageType::StaticClass()  // 기본 데미지 유형
		);
	}
}

void ATakedamageTestActor::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

