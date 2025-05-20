// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArEnemyGameplayAbility.h"
#include "Character/ArEnemyCharacter.h"

AArEnemyCharacter* UArEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedArEnemyCharacter.IsValid())
    {
        CachedArEnemyCharacter = Cast<AArEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedArEnemyCharacter.IsValid() ? CachedArEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UArEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
