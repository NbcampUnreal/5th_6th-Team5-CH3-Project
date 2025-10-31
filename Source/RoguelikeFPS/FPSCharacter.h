// FPSCharacter.h (최종 수정)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

// StatsComponent 및 델리게이트 정의
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, AController*, KillerController);

class USpringArmComponent;
class UCameraComponent;
class UStatsComponent;
class UUserWidget;

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

	// StatsComponent 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatsComponent> StatsComp;

	// UI/델리게이트
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MiniHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnPlayerDeath;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	// 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	// 캐릭터 생존여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	bool bIsAlive = true;

	// Dash 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashMultifly = 10.0f; // 기본값 설정

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashTime = 1.0f; // 기본값 설정

	FTimerHandle DashTimerHandle; // 타이머 핸들러


	// 액션 함수들 (Traditional Input)
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float AxisValue);

	// 나머지 Input 함수
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void LookYaw(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void LookPitch(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartJump();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopJump();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartDash();
	UFUNCTION() // 타이머에서 호출되므로 UFUNCTION 유지
		void StopDash();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartCrouch();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopCrouch();



	// 사망
	void OnDeath(AController* KillerController);

	// 피격 함수 (StatsComp에 위임)
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

};