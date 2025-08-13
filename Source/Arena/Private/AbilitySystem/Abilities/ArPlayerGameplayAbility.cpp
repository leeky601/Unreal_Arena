// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"
#include "Character/ArPlayerCharacter.h"
#include "Controller/ArPlayerController.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "ArenaGameplayTags.h"

AArPlayerCharacter* UArPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
    if (!CachedArPlayerCharacter.IsValid())
    {
        CachedArPlayerCharacter = Cast<AArPlayerCharacter>(CurrentActorInfo->AvatarActor);
    }
    

    return CachedArPlayerCharacter.IsValid()? CachedArPlayerCharacter.Get() : nullptr;
}

AArPlayerController* UArPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
    if (!CachedArPlayerController.IsValid())
    {
        CachedArPlayerController = Cast<AArPlayerController>(CurrentActorInfo->PlayerController);
    }

    return CachedArPlayerController.IsValid() ? CachedArPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UArPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{       
    return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UArPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttckTypeTag, int32 InUsedAttackCombo)
{
    check(EffectClass);

    FGameplayEffectContextHandle EffectContextHandle = GetArAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    EffectContextHandle.SetAbility(this);
    EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    EffectContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetArAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass, GetAbilityLevel(), EffectContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        ArenaGameplayTags::Shared_SetByCaller_BaseDamage,
        InWeaponBaseDamage
    );

    if (InCurrentAttckTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttckTypeTag, InUsedAttackCombo);
    }
  

    return EffectSpecHandle;
}

bool UArPlayerGameplayAbility::GetAbilityCoolDownRemaingByTag(FGameplayTag InCoolDownTag, float& TotalTime, float& RemainingTime)
{
    check(InCoolDownTag.IsValid());

    FGameplayEffectQuery CoolDownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCoolDownTag.GetSingleTagContainer());

    TArray< TPair < float, float> > CoolDownRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CoolDownQuery);

    if (!CoolDownRemainingAndDuration.IsEmpty())
    {
        RemainingTime = CoolDownRemainingAndDuration[0].Key;
        TotalTime = CoolDownRemainingAndDuration[0].Value;
    }

    return RemainingTime > 0.f;
}
