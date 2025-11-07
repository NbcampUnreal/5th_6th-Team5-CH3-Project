// Fill out your copyright notice in the Description page of Project Settings.


#include "TakedamageTestActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATakedamageTestActor::ATakedamageTestActor()
{
	//PrimaryActorTick.bCanEverTick = true;

	//// 루트 컴포넌트 생성 및 설정
	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	//// 충돌 컴포넌트 생성 및 설정
	//Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//// 겹침만 감지하는 프로파일 설정
	//Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//// 루트 컴포넌트로 설정
	//Collision->SetupAttachment(Scene);

	//// 스태틱 메시 컴포넌트 생성 및 설정
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(Collision);
	//// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

    PrimaryActorTick.bCanEverTick = false;

    // 콜리전 설정 (Sphere or Capsule)
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    Collision->InitSphereRadius(5.0f);
    Collision->SetCollisionProfileName(TEXT("Projectile"));
    Collision->SetNotifyRigidBodyCollision(true); // ★ 반드시 필요 (Hit 이벤트 활성화)
    Collision->SetGenerateOverlapEvents(false);   // ★ 오버랩 비활성화
    RootComponent = Collision;

    //// 이동 컴포넌트 (선택)
    //ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    //ProjectileMovement->InitialSpeed = 3000.f;
    //ProjectileMovement->MaxSpeed = 3000.f;
    //ProjectileMovement->bRotationFollowsVelocity = true;
    //ProjectileMovement->bShouldBounce = false;

    // OnHit 바인딩
    //Collision->OnComponentHit.AddDynamic(this, &ATakedamageTestActor::OnProjectileHit);

}

void ATakedamageTestActor::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(Collision))
	{
        Collision->OnComponentHit.AddDynamic(this, &ATakedamageTestActor::OnProjectileHit);
		//Collision->OnComponentEndOverlap.AddDynamic(this, &ATakedamageTestActor::OnItemEndOverlap);
	}
}

void ATakedamageTestActor::OnProjectileHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    if (!OtherActor->ActorHasTag(TEXT("Enemy"))) return;

    const FVector ShotDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    // 피격 데미지 적용
    UGameplayStatics::ApplyPointDamage(
        OtherActor,
        200.f,                 // 데미지 값
        ShotDir,               // 피격 방향
        Hit,                   // Bone 정보 포함된 Hit 결과
        GetInstigatorController(),
        this,
        UDamageType::StaticClass()
    );

    // 총알 제거
    Destroy();
}

//void ATakedamageTestActor::OnItemOverlap(
//    UPrimitiveComponent* OverlappedComp,
//    AActor* OtherActor,
//    UPrimitiveComponent* OtherComp,
//    int32 OtherBodyIndex,
//    bool bFromSweep,
//    const FHitResult& SweepResult)
//{
//    if (!OtherActor || !OtherActor->ActorHasTag(TEXT("Enemy"))) return;
//
//    FHitResult Hit = SweepResult;
//
//    // 1. 기본 보정 (Sweep이 비어있을 경우)
//    if (!Hit.bBlockingHit)
//    {
//        Hit.Component = OtherComp;
//        Hit.Location = OtherActor->GetActorLocation();
//        Hit.ImpactPoint = Hit.Location;
//        Hit.bBlockingHit = true;
//    }
//
//    const FVector ShotDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
//
//    // 2. Overlap 상태에서도 BoneName을 정밀하게 계산
//    if (USkeletalMeshComponent* Skel = Cast<USkeletalMeshComponent>(OtherComp))
//    {
//        FVector CheckPoint = Hit.ImpactPoint.IsNearlyZero()
//            ? OverlappedComp->GetComponentLocation()
//            : Hit.ImpactPoint;
//
//        // head 관련 본 이름들 (상황에 따라 수정 가능)
//        TArray<FName> HeadBones = { TEXT("head"), TEXT("Head"), TEXT("Head_top") };
//
//        FName ClosestBone = NAME_None;
//        float ClosestDistSq = MAX_FLT;
//
//        for (const FName& Bone : HeadBones)
//        {
//            if (!Skel->DoesSocketExist(Bone)) continue;
//
//            const FVector BoneLoc = Skel->GetBoneLocation(Bone);
//            const float DistSq = FVector::DistSquared(BoneLoc, CheckPoint);
//
//            if (DistSq < ClosestDistSq)
//            {
//                ClosestDistSq = DistSq;
//                ClosestBone = Bone;
//            }
//        }
//
//        if (ClosestBone != NAME_None)
//        {
//            Hit.BoneName = ClosestBone;
//
//            DrawDebugSphere(GetWorld(), Skel->GetBoneLocation(ClosestBone), 8.f, 8, FColor::Green, false, 2.f);
//            UE_LOG(LogTemp, Log, TEXT("Manual Closest Bone: %s (dist=%.2f)"),
//                *ClosestBone.ToString(), FMath::Sqrt(ClosestDistSq));
//        }
//        else
//        {
//            // 자동 보정이 안 된 경우, FindClosestBone으로 보조 탐색
//            Hit.BoneName = Skel->FindClosestBone(CheckPoint);
//            UE_LOG(LogTemp, Warning, TEXT("Fallback FindClosestBone -> %s"), *Hit.BoneName.ToString());
//        }
//    }
//
//    // 3. 데미지 적용
//    UGameplayStatics::ApplyPointDamage(
//        OtherActor,
//        200.f,
//        ShotDir,
//        Hit,
//        GetInstigatorController(),
//        this,
//        UDamageType::StaticClass()
//    );
//
//    // 4. 디버그 출력
//    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
//        FString::Printf(TEXT("ApplyPointDamage → %s (Bone=%s)"),
//            *GetNameSafe(OtherActor),
//            *Hit.BoneName.ToString()));
//}




//void ATakedamageTestActor::OnItemEndOverlap(
//	UPrimitiveComponent* OverlappedComp,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32 OtherBodyIndex)
//{
//}

