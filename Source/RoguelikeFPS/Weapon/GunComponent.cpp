// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileComponent.h"
#include "Engine/SCS_Node.h"

UGunComponent::UGunComponent()
{

}

void UGunComponent::DoAttack()
{
	if (_Character == nullptr || _Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (_ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = this->GetSocketLocation(FName("Muzzle"));
			
			//const FVector SpawnLocation = GetOwner()->GetActorLocation();
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("GetActorLocation() : %s"), *GetOwner()->GetActorLocation().ToString()));
			GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, FString::Printf(TEXT("SpawnLocation : %s"), *SpawnLocation.ToString()));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AActor>(_ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// Try and play the sound if specified b 
	{
		UGameplayStatics::PlaySoundAtLocation(this, _AttackSound, _Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (_AttackAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = _Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
	if (_AttackSound != nullptr)
			AnimInstance->Montage_Play(_AttackAnimation, 1.f);
		}
	}
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Check_ProjectileHaveComponent())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Gun's Projectile does not have Component"));
	}
	
	
	
	//Begin = true;
}


void UGunComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UGunComponent::Check_ProjectileHaveComponent()
{
	if (UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(_ProjectileClass))
	{
		if (USimpleConstructionScript* SCS = BPGC->SimpleConstructionScript)
		{
			for (USCS_Node* Node : SCS->GetAllNodes())
			{
				if (!Node) continue;

				// 실제 생성될 컴포넌트 클래스를 얻음
				UClass* CompClass = Node->ComponentClass;
				if (!CompClass) continue;

				// 비교: UProjectileComponent의 서브클래스인지 확인
				if (CompClass->IsChildOf(UProjectileComponent::StaticClass()))
				{
					return true;
				}
			}
		}
	}

	return false;
}
