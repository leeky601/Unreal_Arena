// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Wepons/ArPlayerWeapon.h"

AArPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AArPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
