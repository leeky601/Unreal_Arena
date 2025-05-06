// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Wepons/ArWeaponBase.h"
#include "ArenaTypes/ArenaStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "ArPlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API AArPlayerWeapon : public AArWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FArenaPlayerWeaponData PlayerWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles();

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
