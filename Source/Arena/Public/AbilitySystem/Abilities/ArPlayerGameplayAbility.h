// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ArGameplayAbility.h"
#include "ArPlayerGameplayAbility.generated.h"

class AArPlayerCharacter;
class AArPlayerController;
/**
 * 
 */
UCLASS()
class ARENA_API UArPlayerGameplayAbility : public UArGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	AArPlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	AArPlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arena|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AArPlayerCharacter> CachedArPlayerCharacter;
	TWeakObjectPtr<AArPlayerController> CachedArPlayerController;
};
