
#pragma once

#include "GameplayTagContainer.h"

#define USTAG_CHARACTER_EQUIP_WEAPON FGameplayTag::RequestGameplayTag(FName("Character.Equip.Weapon"))
#define USTAG_CHARACTER_EQUIP_SHIELD FGameplayTag::RequestGameplayTag(FName("Character.Equip.Shield"))
#define USTAG_CHARACTER_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.Invinsible"))
#define USTAG_CHARACTER_INVINSIBLE_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("Character.Invinsible.AttackHit"))
#define USTAG_CHARACTER_STATE_COMBAT FGameplayTag::RequestGameplayTag(FName("Character.State.Combat"))
#define USTAG_CHARACTER_STATE_DEATH FGameplayTag::RequestGameplayTag(FName("Character.State.Death"))
#define USTAG_INPUT_STATE_DEATH FGameplayTag::RequestGameplayTag(FName("Input.State.Death"))
#define USTAG_INPUT_STATE_COMBAT FGameplayTag::RequestGameplayTag(FName("Input.State.Combat"))
#define USTAG_INPUT_ATTACK_COMBOATTAC1 FGameplayTag::RequestGameplayTag(FName("Input.Attack.ComboAttack1"))

