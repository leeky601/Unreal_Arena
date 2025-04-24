// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"
#include "Character/ArPlayerCharacter.h"
#include "Controller/ArPlayerController.h"

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
