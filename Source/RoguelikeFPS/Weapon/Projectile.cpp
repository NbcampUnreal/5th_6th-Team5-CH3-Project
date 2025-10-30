// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->InitSphereRadius(5.0f);
	_Collision->SetCollisionProfileName("OverlapAll");
	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	_Collision->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	_Collision->CanCharacterStepUpOn = ECB_No;

	RootComponent = _Collision;

	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	_ProjectileMovement->UpdatedComponent = _Collision;
	_ProjectileMovement->InitialSpeed = 3000.f;
	_ProjectileMovement->MaxSpeed = 3000.f;
	_ProjectileMovement->bRotationFollowsVelocity = true;
	_ProjectileMovement->bShouldBounce = true;
	_ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green , FString::Printf(TEXT("TempActor Name : %s"), *_Instigator->GetName()));
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//OnDestroyed.Broadcast();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;
	if (OtherActor == GetOwner()) return; // 소유자 자신 무시
	if (OtherActor->IsA(AProjectile::StaticClass())) return;
	
	if (IsValid(_Instigator))
	{
		if (_Instigator != OtherActor)
		{
			Destroy();
		}
	}
}


void AProjectile::SetMovementSpeed(float speed)
{
	if (speed < 0) return;
	_ProjectileMovement->InitialSpeed = speed;
	_ProjectileMovement->MaxSpeed = speed;
}

void AProjectile::SetInstigator(AActor* instigator)
{
	if (instigator) {
		GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("SetInstigator")));
		_Instigator = instigator;
	}
}

void AProjectile::SetDamage(float damage)
{
	if (0 < damage)
	{
		_Damage = damage;
	}
}

