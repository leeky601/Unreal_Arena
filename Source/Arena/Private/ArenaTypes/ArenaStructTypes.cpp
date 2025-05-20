// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaTypes/ArenaStructTypes.h"
#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"

bool FArenaPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
