// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Skill/Missile.h"
#include "Weapon/GunComponent.h"
#include "Weapon/Skill/MissileActor.h"
#include "Kismet/GameplayStatics.h"

UMissile::UMissile()
{
    _Collision = nullptr;
}

void UMissile::Active()
{
    Super::Active();

    if (_isSkillCoolDown) return;
    _isSkillCoolDown = true;

    CreateCollision();

    FTimerHandle CollisionDestoryHandle;
    GetWorld()->GetTimerManager().SetTimer(CollisionDestoryHandle, FTimerDelegate::CreateLambda([this]() {
        DestroyCollision();
        SpawnMissiles();
        OverlappedCharacters.Empty();
        //GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("DestroyCollision")));
        }), 1.0f, false);
}

void UMissile::SetUp()
{
    Super::SetUp();

    if (GetAttachParent())
    {
        UGunComponent* Guncomp = Cast<UGunComponent>(GetAttachParent());
        if (Guncomp) _Instigator = Guncomp->GetOwnerCharacter();
    }
}

void UMissile::OnCollsionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
    if (!IsValid(_Instigator)) return;
    if (_Instigator == OtherActor) return;

    ACharacter* HitChar = Cast<ACharacter>(OtherActor);
    if (!HitChar) return;
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("UMissile :: OnCollsionBeginOverlap")));
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("UMissile :: HitChar Name : %s"), *HitChar->GetName()));
    OverlappedCharacters.Add(HitChar);
}

void UMissile::CreateCollision()
{   
    if (!IsValid(_Collision)) _Collision = NewObject<UBoxComponent>(_Instigator, UBoxComponent::StaticClass(), TEXT("Collision"));

    if (_Collision)
    {
        USceneComponent* ParentComp = _Instigator->GetRootComponent();

        _Collision->InitBoxExtent(FVector(1000.0f, 1000.0f, 70.0f));
        _Collision->SetMobility(EComponentMobility::Movable);
        _Collision->SetHiddenInGame(false);
        _Collision->SetGenerateOverlapEvents(true);
        _Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        _Collision->SetCollisionProfileName(TEXT("OverlapAll"));
        
        _Collision->AttachToComponent(ParentComp, FAttachmentTransformRules::KeepRelativeTransform);

        _Collision->OnComponentBeginOverlap.RemoveAll(this);
        _Collision->OnComponentBeginOverlap.AddDynamic(this, &UMissile::OnCollsionBeginOverlap);

        if (!_Collision->IsRegistered())
        {
            _Collision->RegisterComponent();
        }
    }
}

void UMissile::DestroyCollision()
{
    if (_Collision)
    {
        _Collision->OnComponentBeginOverlap.RemoveAll(this);
        _Collision->DestroyComponent();
        _Collision = nullptr;
    }
}

void UMissile::SpawnMissiles()
{
    if (OverlappedCharacters.Num() <= 0) return;
    if (_Instigator)
    {
        const FRotator SpawnRotation(0.0f, 90.f, 0.0f);
        const FVector SpawnLocation(_Instigator->GetActorLocation());

        ////Set Spawn Collision Handling Override
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        TArray<TWeakObjectPtr<AMissileActor>> MissilesToFire;
        MissilesToFire.SetNum(_MissilesToFireCount);

        UWorld* const World = GetWorld();
        for (size_t i = 0; i < _MissilesToFireCount; i++)
        {
            if (_TSubMissileActor == nullptr) return;
            MissilesToFire[i] = World->SpawnActorDeferred<AMissileActor>(_TSubMissileActor, FTransform(SpawnRotation, SpawnLocation));
            if (MissilesToFire[i] != nullptr)
            {
                FVector Location = SpawnLocation;
                Location.X += FMath::FRandRange(-100.f, 100.f);
                Location.Y += FMath::FRandRange(-100.f, 100.f);
                Location.Z += FMath::FRandRange(0.f, 100.f);
                MissilesToFire[i]->SetInstigator(_Instigator);
                MissilesToFire[i]->SetTarget(Cast<AActor>(OverlappedCharacters[FMath::RandRange(0, OverlappedCharacters.Num() - 1)]));
                MissilesToFire[i]->SetDamage(_Damage);
                UGameplayStatics::FinishSpawningActor(MissilesToFire[i].Get(), FTransform(SpawnRotation, Location));
            }
        }
    }
}
