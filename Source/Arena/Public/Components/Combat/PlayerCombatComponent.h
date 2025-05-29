// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class AArPlayerWeapon;
/**
 * 
 */
UCLASS()
class ARENA_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	AArPlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	AArPlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	float GetWeaponDamage(float Inlevel) const;

	virtual void OnHitTargetActor(AActor* TargetActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* TargetActor) override;
};
