// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/Flamethrower.h"
#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/BodySetup.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UFlamethrower::UFlamethrower()
{
    _Collision = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Collision"));
    _Collision->SetCollisionProfileName("Projectile");
    _Collision->OnComponentBeginOverlap.AddDynamic(this, &UFlamethrower::OnCollsionBeginOverlap);
    _Collision->OnComponentEndOverlap.AddDynamic(this, &UFlamethrower::OnCollsionEndOverlap);
    _Collision->SetVisibility(false);
    _Collision->SetGenerateOverlapEvents(false);

    _VFXComponent = nullptr;
}

void UFlamethrower::Active()
{
	Super::Active();

    if (_isSkillCoolDown) return;
    if (_Collision->GetGenerateOverlapEvents()) return;

    _Collision->SetGenerateOverlapEvents(true);
    _isSkillCoolDown = true;
    if (_VFXComponent) _VFXComponent->Activate();
    

    float time = 1.0f / (float)_TickPerSecond;

    UWorld* World = GetWorld();
    if (!World) return;
    World->GetTimerManager().SetTimer(
        DamageOnTickHandle,
        this,
        &UFlamethrower::ApplyDamageOnTick,
        time,
        true // 반복
    );

    FTimerHandle FlamethrowerDurationHandle;
    World->GetTimerManager().SetTimer(FlamethrowerDurationHandle, FTimerDelegate::CreateLambda([this]() {
        _Collision->SetGenerateOverlapEvents(false);
        _isSkillCoolDown = false;
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("SetGenerateOverlapEvents : false")));
        }), _FlamethrowerDuration, false);

    //GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("UFlamethrower :: Active")));

}

void UFlamethrower::SetUp()
{
    Super::SetUp();    

    if (_Collision)
    {
        CreateCollisonShape();        

        if (GetAttachParent())
        {
            /*FVector Direction = (RibbonStart->GetRelativeLocation()) - (RibbonEnd->GetRelativeLocation());
            RibbonParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(RibbonParticle, RibbonStart, NAME_None, -(Direction / 2), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false, false);*/
            
            _Collision->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Muzzle"));
            UGunComponent* Guncomp = Cast<UGunComponent>(GetAttachParent());
            if (Guncomp) _Instigator = Guncomp->GetOwnerCharacter();
            if (_VFXFlamethrower) {
                _VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(_VFXFlamethrower, _Collision, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false, false);
                _VFXComponent->SetVectorParameter(TEXT("Length"), FVector(_Length, 0, 0));
                float Degrees = FMath::RadiansToDegrees(FMath::Atan(_Width * 0.5/ _Length));
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Degrees : %f"), Degrees));
                _VFXComponent->SetVectorParameter(TEXT("MaximumRotation"), FVector(_Angle * 0.5, Degrees * 0.5, 0));
                _VFXComponent->SetVectorParameter(TEXT("MinimumRotation"), FVector(-_Angle * 0.5, -Degrees * 0.5, 0));
                //RibbonParticleComponent->SetVectorParameter(TEXT("Direction"), Direction);  
            }
        }
    }
}

void UFlamethrower::OnCollsionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!_Collision->GetGenerateOverlapEvents()) return;
    if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
    if (!IsValid(_Instigator)) return;
    if (_Instigator == OtherActor) return;

    ACharacter* HitChar = Cast<ACharacter>(OtherActor);
    if (!HitChar) return;
    OverlappedCharacters.Add(HitChar);
}

void UFlamethrower::OnCollsionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!_Collision->GetGenerateOverlapEvents()) return;
    if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
    if (!IsValid(_Instigator)) return;
    if (_Instigator == OtherActor) return;

    ACharacter* HitChar = Cast<ACharacter>(OtherActor);
    if (!HitChar) return;
    OverlappedCharacters.Remove(HitChar);
}

void UFlamethrower::CreateCollisonShape()
{
    if (!_Collision) return;

    UE_LOG(LogTemp, Warning, TEXT("CreateCollisonShape"));

    _Collision->ClearAllMeshSections();

    float Radians = FMath::DegreesToRadians(_Angle * 0.5);
    _Width = _Length * FMath::Tan(Radians);

    const float HalfWidth = _Width;
    const float HalfHegiht = _Height * 0.5f;

    // 정점: 밑면 4개 + 꼭대기 1개 = 5개
    TArray<FVector> Vertices;
    Vertices.Add(FVector(_Length, -HalfWidth, -HalfHegiht)); // 0: 좌하
    Vertices.Add(FVector(_Length, HalfWidth, -HalfHegiht));  // 1: 우하
    Vertices.Add(FVector(_Length, HalfWidth, HalfHegiht));   // 2: 우상
    Vertices.Add(FVector(_Length, -HalfWidth, HalfHegiht));  // 3: 좌상
    Vertices.Add(FVector(0.f, 0.f, 0.f)); // 4: 꼭대기

    TArray<int32> Triangles;
    Triangles.Append({ 0, 2, 1 });
    Triangles.Append({ 0, 3, 2 });
    Triangles.Append({ 0, 1, 4 });
    Triangles.Append({ 1, 2, 4 });
    Triangles.Append({ 2, 3, 4 });
    Triangles.Append({ 3, 0, 4 });

    TArray<FVector> EmptyNormals;
    TArray<FVector2D> EmptyUV0;
    TArray<FLinearColor> VertexColors;
    VertexColors.Init(FLinearColor::White, Vertices.Num());
    TArray<FProcMeshTangent> EmptyTangents;

    // 섹션 생성 (bCreateCollision = true이면 콜리전 데이터 생성)
    _Collision->CreateMeshSection_LinearColor(0, Vertices, Triangles, EmptyNormals, EmptyUV0, VertexColors, EmptyTangents, false);

    _Collision->bUseComplexAsSimpleCollision = false;
    _Collision->bUseAsyncCooking = true;
    
    TArray<FVector> ConvexVerts = Vertices;
    // 기존 convex 지우고 추가
    _Collision->ClearCollisionConvexMeshes();
    _Collision->AddCollisionConvexMesh(ConvexVerts);

    if (!_Collision->IsRegistered()) {
        _Collision->RegisterComponent();
    }
}

void UFlamethrower::ApplyDamageOnTick()
{
    if (!_Collision->GetGenerateOverlapEvents())
    {
        GetWorld()->GetTimerManager().ClearTimer(DamageOnTickHandle);
        if (_VFXComponent) _VFXComponent->Deactivate();
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("ApplyDamageOnTick")));

    for (auto& character : OverlappedCharacters)
    {
        //character->TakeDamage();
    }
}
