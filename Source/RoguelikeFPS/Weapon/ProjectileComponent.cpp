// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

UProjectileComponent::UProjectileComponent()
{
	SphereRadius = 5.0f;
	SetCollisionProfileName("Projectile");
	OnComponentHit.AddDynamic(this, &UProjectileComponent::OnHit);
	SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CanCharacterStepUpOn = ECB_No;

	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	_ProjectileMovement->InitialSpeed = 3000.f;
	_ProjectileMovement->MaxSpeed = 3000.f;
	_ProjectileMovement->bRotationFollowsVelocity = true;
	_ProjectileMovement->bShouldBounce = true;
	_ProjectileMovement->ProjectileGravityScale = 0.f;
}

void UProjectileComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner())
	{
		//if ((OtherActor != nullptr) && (OtherActor != GetOwner()) && (OtherComp != nullptr))
		if ((OtherActor != nullptr) && (OtherActor != GetOwner()) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			//OtherComp->AddImpulseAtLocation(GetOwner()->GetVelocity() * 100.0f, GetOwner()->GetActorLocation());

			GetOwner()->Destroy();
		}
	}
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		GetOwner()->SetLifeSpan(3.0f);
		_ProjectileMovement->UpdatedComponent = GetOwner()->GetRootComponent();
	}
}

void UProjectileComponent::SetMovementSpeed(float speed)
{
	_ProjectileMovement->InitialSpeed = speed;
	_ProjectileMovement->MaxSpeed = speed;
}
