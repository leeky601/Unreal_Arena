// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilites, InASCToGive, ApplyLevel);

	GrantGameEffects(StartUpGameEffects, InASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UArGameplayAbility>>& InAbilitiesToGive, UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UArGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}
}

void UDataAsset_StartUpDataBase::GrantGameEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffectsToGive, UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (!InEffectsToGive.IsEmpty())
	{
		for(const TSubclassOf<UGameplayEffect>& EffectClass: InEffectsToGive)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
				);
		}
	}
}
