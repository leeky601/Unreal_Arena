// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ArGameplayAbility.h"
#include "ArEnemyGameplayAbility.generated.h"

class AArEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class ARENA_API UArEnemyGameplayAbility : public UArGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	AArEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalabeFloat) ;

private:
	TWeakObjectPtr<AArEnemyCharacter> CachedArEnemyCharacter;
};
