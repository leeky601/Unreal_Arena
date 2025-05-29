// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Wepons/ArPlayerWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaGameplayTags.h"

#include "DebugHelper.h"

AArPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AArPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AArPlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
    return Cast<AArPlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetWeaponDamage(float Inlevel) const
{
    return GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(Inlevel);
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* TargetActor)
{
    if (OverlappedActors.Contains(TargetActor)) return;

    OverlappedActors.AddUnique(TargetActor);
    
    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = TargetActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(), 
        ArenaGameplayTags::Shared_Event_MeleeHit,
        Data);
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* TargetActor)
{
   
}


