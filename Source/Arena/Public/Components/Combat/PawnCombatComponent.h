// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AArWeaponBase;
/**
 * 
 */
UCLASS()
class ARENA_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	void RegisterSpawnWeapon(FGameplayTag InWeaponTagToRegister, AArWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeappon = false);

	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	AArWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Arena|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	AArWeaponBase* GetCharacterCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AArWeaponBase*> CharacterCarriedWeaponMap;
};
