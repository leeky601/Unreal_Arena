// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentEquippedChangedDelegate, TSoftObjectPtr<UTexture2D>, ChangedWeaponTexture);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>,AbilityIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldown, float, RemainCooldown);


/**
 * 
 */
UCLASS()
class ARENA_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCurrentEquippedChangedDelegate OnCurrentEquippedChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityIconUpdatedDelegate OnAbilityIconUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin ;
};
