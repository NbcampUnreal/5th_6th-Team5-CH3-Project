// FPSCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

// **[추가]** AController 포인터를 포함하는 사망 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, AController*, KillerController);

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

	// **[추가]** 사망 델리게이트 변수
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnPlayerDeath;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// **[추가]** BeginPlay 선언 (필요할 수 있으므로 유지)
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	// Status (유지)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Level;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Defence;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 AttackSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 MovingSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 Experience;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int32 MaxExperience;
	// 캐릭터 생존여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	bool bIsAlive;


	// 대시 속도 (유지)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashMultifly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashTime;


	// 대시 타이머 핸들러 (유지)
	FTimerHandle DashTimerHandle;


	// **[수정]** Action Value 함수 시그니처를 전통적인 방식 (float) 또는 단순 함수로 변경
	// Blueprint에서 바인딩할 수 있도록 UFUNCTION과 BlueprintCallable을 추가합니다.
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float AxisValue);
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
	UFUNCTION()
	void StopDash();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartCrouch();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopCrouch();

	// 레벨업
	void LevelUp();

	// 사망
	void OnDeath(AController* KillerController); // **[수정]** KillerController를 받도록 시그니처 변경

	// 피격 함수
	// **[수정]** UHT 오류 방지를 위해 원시 포인터 AController*와 AActor*를 사용
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

};