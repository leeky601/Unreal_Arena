// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArEnemyGameplayAbility.h"
#include "Character/ArEnemyCharacter.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "ArenaGameplayTags.h"

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

FGameplayEffectSpecHandle UArEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalabeFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle EffectContextHandle = GetArAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    EffectContextHandle.SetAbility(this);
    EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    EffectContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetArAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass, GetAbilityLevel(), EffectContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(ArenaGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalabeFloat.GetValueAtLevel(GetAbilityLevel()));

    return EffectSpecHandle;
}
