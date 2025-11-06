// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TakedamageTestActor.generated.h"

class USphereComponent;

UCLASS()
class ROGUELIKEFPS_API ATakedamageTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATakedamageTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 루트 컴포넌트 (씬)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	// 충돌 컴포넌트 (플레이어 진입 범위 감지용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	// 아이템 시각 표현용 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;

	//UFUNCTION()
	//virtual void OnItemOverlap(
	//	UPrimitiveComponent* OverlappedComp,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool bFromSweep,
	//	const FHitResult& SweepResult);
	//UFUNCTION()
	//virtual void OnItemEndOverlap(
	//	UPrimitiveComponent* OverlappedComp,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex);

	UFUNCTION()
	void OnProjectileHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
