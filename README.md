# RoguelikeFPS

## 프로젝트 개요

- **프로젝트 이름**: RoguelikeFPS  
- **장르**: 로그라이크 FPS + AI 전투 시스템  
- **엔진 버전**: Unreal Engine 5.5.4  
- **개발 기간**: 2025.10.20 ~ 2025.11.10  
- **팀 구성**
  - **전민성**: 팀장, 기획, AI, 레벨 디자인 
  - **김재홍**: 게임 루프, UI
  - **김하빈**: 무기, 스킬
  - **문승재**: 인벤토리, 제작, 강화, 아이템 장착 
  - **엄상민**: 플레이어 캐릭터, 맵 디자인

### 주요 특징
- Behavior Tree, Finite State Machine 기반의 AI 제어
- ItemBase, ItemDataTable을 이용한 아이템 DB관리
- Unreal Component 구조를 이용한 WeaponSkillComponent 구현
- 게임 시작 및 종료 이벤트 컨트롤
- 이벤트 발생시 Widget을 통해 플레이어에게 정보 전달을 할 수 있도록 설계

## 프로젝트 구조
```
/Source/RoguelikeFPS/
│ FPSCharacter.cpp / FPSCharacter.h
│ FPSGameMode.cpp / FPSGameMode.h
│ FPSPlayerController.cpp / FPSPlayerController.h
│ PlayerCharacter.cpp / PlayerCharacter.h
│ RoguelikeFPS.cpp / RoguelikeFPS.h
│
├─ AI
│ │ DropTable.cpp / DropTable.h
│ │
│ ├─ Character
│ │ AIEnemyCharacter.cpp / AIEnemyCharacter.h
│ │ AIEnemyController.cpp / AIEnemyController.h
│ │
│ ├─ Node
│ │ BTTask_ClearBBKey.cpp / BTTask_ClearBBKey.h
│ │ BTTask_ClearLastSeen.cpp / BTTask_ClearLastSeen.h
│ │ BTTask_FindPatrolPoint.cpp / BTTask_FindPatrolPoint.h
│ │ BTTask_SetMoveSpeed.cpp / BTTask_SetMoveSpeed.h
│ │ BTTask_SetState.cpp / BTTask_SetState.h
│ │
│ ├─ Structure
│ │ │ EnemyAnimInstance.cpp / EnemyAnimInstance.h
│ │ │ EnemyConfig.cpp / EnemyConfig.h
│ │ │ EnemyState.cpp / EnemyState.h
│ │ │
│ │ ├─ AnimNotify
│ │ │ AnimNotify_AttackExit.cpp / AnimNotify_AttackExit.h
│ │ │ AnimNotify_AttackHit.cpp / AnimNotify_AttackHit.h
│ │ │ AnimNotify_Boss1.cpp / AnimNotify_Boss1.h
│ │ │ AnimNotify_Boss2.cpp / AnimNotify_Boss2.h
│ │ │ AnimNotify_Boss3.cpp / AnimNotify_Boss3.h
│ │ │ AnimNotify_DeadExit.cpp / AnimNotify_DeadExit.h
│ │ │
│ │ └─ Component
│ │ EnemyAttackBaseComponent.cpp / EnemyAttackBaseComponent.h
│ │ EnemyStateMachineComponent.cpp / EnemyStateMachineComponent.h
│ │ EnemyStatsComponent.cpp / EnemyStatsComponent.h
│ │ MeleeAttackComponent.cpp / MeleeAttackComponent.h
│ │ RangedAttackComponent.cpp / RangedAttackComponent.h
│ │ Stage2BossAttackComponent.cpp / Stage2BossAttackComponent.h
│ │
│ └─ Test
│ TakedamageTestActor.cpp / TakedamageTestActor.h
│
├─ InventoryWidget
│ InventoryItemWidget.cpp / InventoryItemWidget.h
│ InventorySlotWidget.cpp / InventorySlotWidget.h
│ ShopItemWidget.cpp / ShopItemWidget.h
│ ShopWidget.cpp / ShopWidget.h
│
├─ Items
│ PotionItem.cpp / PotionItem.h
│
├─ Private / Public
│ AugmentWidget.cpp / AugmentWidget.h
│ CraftingMenuWidget.cpp / CraftingMenuWidget.h
│ CraftingSystem.cpp / CraftingSystem.h
│ CraftingWidget.cpp / CraftingWidget.h
│ DeathWidget.cpp / DeathWidget.h
│ DropItem.cpp / DropItem.h
│ GameClearWidget.cpp / GameClearWidget.h
│ GameDataInstance.cpp / GameDataInstance.h
│ Inventory.cpp / Inventory.h
│ InventoryWidget.cpp / InventoryWidget.h
│ ItemAlbum.cpp / ItemAlbum.h
│ ItemBase.cpp / ItemBase.h
│ ItemData.cpp / ItemData.h
│ KillFeedComponent.cpp / KillFeedComponent.h
│ MainMenuController.cpp / MainMenuController.h
│ MainMenuGameMode.cpp / MainMenuGameMode.h
│ MainMenuWidget.cpp / MainMenuWidget.h
│ Making.cpp / Making.h
│ MyCharacter.cpp / MyCharacter.h
│ PartMenuWidget.cpp / PartMenuWidget.h
│ PartSystem.cpp / PartSystem.h
│ PartWidget.cpp / PartWidget.h
│ ResourceItem.cpp / ResourceItem.h
│ Shop.cpp / Shop.h
│ StatsComponent.cpp / StatsComponent.h
│ StatsHUD.cpp / StatsHUD.h
│ TeleportCountdownWidget.cpp / TeleportCountdownWidget.h
│ TeleportVolume.cpp / TeleportVolume.h
│ TestCraftingActor.cpp / TestCraftingActor.h
│ TitleWidget.cpp / TitleWidget.h
│ UpgradeMenuWidget.cpp / UpgradeMenuWidget.h
│ UpgradeSystem.cpp / UpgradeSystem.h
│ UpgradeWidget.cpp / UpgradeWidget.h
│
├─ Weapon
│ │ GunComponent.cpp / GunComponent.h
│ │ GunStatus.cpp / GunStatus.h
│ │ PickUpComponent.cpp / PickUpComponent.h
│ │ Projectile.cpp / Projectile.h
│ │ WeaponComponent.cpp / WeaponComponent.h
│ │ WeaponSkillComponent.cpp / WeaponSkillComponent.h
│ │
│ └─ Skill
│ ChanceToStunAbility.cpp / ChanceToStunAbility.h
│ ChargeableDash.cpp / ChargeableDash.h
│ DamageRampUpAbility.cpp / DamageRampUpAbility.h
│ DoubledAmmoBuff.cpp / DoubledAmmoBuff.h
│ DoubleShotAbility.cpp / DoubleShotAbility.h
│ ExplosiveActor.cpp / ExplosiveActor.h
│ ExplosiveBullet.cpp / ExplosiveBullet.h
│ Flamethrower.cpp / Flamethrower.h
│ HeadshotGainAmmoAbility.cpp / HeadshotGainAmmoAbility.h
│ IncreaseHaedShotDamage.cpp / IncreaseHaedShotDamage.h
│ IncreaseMoveSpeed.cpp / IncreaseMoveSpeed.h
│ InvincibilityAbility.cpp / InvincibilityAbility.h
│ Missile.cpp / Missile.h
│ MissileActor.cpp / MissileActor.h
│ QuickReloadBuff.cpp / QuickReloadBuff.h
│ ShieldGainOnAttack.cpp / ShieldGainOnAttack.h
│ SixShot.cpp / SixShot.h
│ UnlimitedAmmoAbility.cpp / UnlimitedAmmoAbility.h
│
└─ WeaponPart
PartData.cpp / PartData.h
PartItem.cpp / PartItem.h
TestWeapon.cpp / TestWeapon.h
```

## 실행 방법

에셋의 문제로 링크 공유 불가


## 향후 개선 계획
- UI 개선
- AI 행동 유연화
- 보스 패턴 범위 표시
- 증강체 개선

