// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ExplosiveBullet.h"
#include "Weapon/GunComponent.h"

void UExplosiveBullet::SetUp()
{
	//InitialLifeSpan = 3.0f;
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->ProjectileSpawn.AddDynamic(this, &UExplosiveBullet::Projectile_AddDynamic);
		//GunComp->GetProjectile()->OnDestroyed.AddDynamic(this, &UExplosiveBullet::Active1);
		//OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnSphereBeginOverlap);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("SetUp : AddDynamic")));
	}
}

void UExplosiveBullet::Projectile_AddDynamic(AProjectile* projectile)
{
	if (projectile)
	{
		projectile->OnDestroyed.AddDynamic(this, &UExplosiveBullet::Spawn_Explosion);
	}
}

void UExplosiveBullet::Spawn_Explosion(AActor* DestroyedActor)
{
	if (_Explosion)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			World->SpawnActor<AExplosiveActor>(_Explosion, DestroyedActor->GetActorLocation(), DestroyedActor->GetActorRotation(), ActorSpawnParams);
		}
	}
}
