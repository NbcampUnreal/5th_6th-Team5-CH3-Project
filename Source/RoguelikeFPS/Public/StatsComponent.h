//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "FPSCharacter.generated.h"
//
//class USpringArmComponent;
//class UCameraComponent;
//struct FInputActionValue;
//
//UCLASS()
//class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
//{
//    GENERATED_BODY()
//
//public:
//    AFPSCharacter();
//
//protected:
//    virtual void BeginPlay() override;
//
//public:
//    virtual void Tick(float DeltaTime) override;
//    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//    //  UStatsComponent 멤버들이 캐릭터 스탯으로 통합됨 
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
//    int PlayerLevel = 1;
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
//    float CurrentXP = 0.0f;
//
//    // 이전 코드에 있던 Status 변수들
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 Health = 100;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 MaxHealth = 100;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 Attack = 10;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 Defence = 10;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 AttackSpeed = 5;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 MovingSpeed = 600;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 Stamina = 500;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 Experience = 0;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    int32 MaxExperience = 100;
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
//    bool bIsAlive = true;
//
//    //  Combat State 변수들 (이전 오류 해결을 위해 추가됨) 
//    UPROPERTY(BlueprintReadWrite, Category = "CombatState")
//    bool bIsFiring = false;
//
//    UPROPERTY(BlueprintReadWrite, Category = "CombatState")
//    float FireCooltime = 0.5f;
//
//    UPROPERTY(BlueprintReadWrite, Category = "CombatState")
//    bool bIsDashing2 = false;
//
//    UPROPERTY(BlueprintReadWrite, Category = "CombatState")
//    bool bIsDashing = false;
//
//    UPROPERTY(BlueprintReadWrite, Category = "CombatState")
//    bool bIsReloading = false;
//
//    //  Dash 관련 변수들 
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
//    float DashMultifly = 10.0f;
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
//    float DashSpeed;
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
//    float DashTime = 1.0f;
//    FTimerHandle DashTimerHandle;
//
//    //  카메라 컴포넌트 (이전 코드에 있던 것으로 가정하고 추가) 
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
//    TObjectPtr<USpringArmComponent> SpringArmComp;
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
//    TObjectPtr<UCameraComponent> CameraComp;
//
//    //  Weapon Meshes 
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
//    USkeletalMeshComponent* PistolMesh;
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
//    USkeletalMeshComponent* RifleMesh;
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
//    USkeletalMeshComponent* ShotgunMesh;
//
//
//    // --- 함수 ---
//    void StartFire();
//    void StopFire();
//    void FireWeapon();
//
//    UFUNCTION(BlueprintCallable, Category = "Weapon")
//    void InitializeWeapon(FName SelectedWeaponName);
//
//    UFUNCTION()
//    void Move(const FInputActionValue& value);
//    UFUNCTION()
//    void Look(const FInputActionValue& value);
//    UFUNCTION()
//    void StartJump(const FInputActionValue& value);
//    UFUNCTION()
//    void StopJump(const FInputActionValue& value);
//    UFUNCTION()
//    void StartDash(const FInputActionValue& value);
//    UFUNCTION()
//    void StopDash();
//    UFUNCTION()
//    void StartCrouch(const FInputActionValue& value);
//    UFUNCTION()
//    void StopCrouch(const FInputActionValue& value);
//
//    //  UStatsComponent에서 통합된 함수 
//    UFUNCTION()
//    void LevelUp();
//
//    UFUNCTION()
//    void OnDeath();
//
//    UFUNCTION(BlueprintCallable, Category = "Stats")
//    void LevelUpWithOptions(); // 레벨업 증강 선택 트리거 함수
//
//    UFUNCTION()
//    void HandleDeath(); // 사망 이벤트 처리
//
//    UFUNCTION()
//    void HandleStageClear(); // 스테이지 클리어 이벤트 처리
//
//    UFUNCTION()
//    void AddXP(float AmountToAdd); // 경험치 획득
//
//    UFUNCTION(BlueprintCallable, Category = "Stats")
//    void ApplyAugment(FName AugmentID); // 증강 적용
//
//    // 피격 함수 (ACharacter 오버라이드)
//    virtual float TakeDamage(
//        float DamageAmount,
//        FDamageEvent const& DamageEvent,
//        TObjectPtr<AController> EventInstigator,
//        TObjectPtr<AActor> DamageCauser);
//
//};