// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Wepons/ArPlayerWeapon.h"

void AArPlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AArPlayerWeapon::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}
