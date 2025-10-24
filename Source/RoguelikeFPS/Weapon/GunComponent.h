// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponComponent.h"
#include "GunStatus.h"
#include "GunComponent.generated.h"

/**
 * 
 */


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UGunComponent : public UWeaponComponent
{
	GENERATED_BODY()
	

public:
	UGunComponent();
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	FGunStatus _Status;
	
public:
	virtual void DoAttack() override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	bool Check_ProjectileHaveComponent();
};
