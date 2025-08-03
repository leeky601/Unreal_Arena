// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AArWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquipedWeapon,
	LeftHand,
	RightHand
};
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

	UFUNCTION(BlueprintCallable, Category = "Arena|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquipedWeapon);

	virtual void OnHitTargetActor(AActor* TargetActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* TargetActor);

protected:
	TArray<AActor*> OverlappedActors;

	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

private:
	TMap<FGameplayTag, AArWeaponBase*> CharacterCarriedWeaponMap;
};
