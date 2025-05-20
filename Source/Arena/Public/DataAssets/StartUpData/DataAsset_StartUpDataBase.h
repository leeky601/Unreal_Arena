// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UArGameplayAbility;
class UArAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ARENA_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UArGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UArGameplayAbility>> ReactiveAbilites;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameEffects;

	void GrantAbilities(const TArray<TSubclassOf<UArGameplayAbility>>& InAbilitiesToGive, UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

	void GrantGameEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffectsToGive, UArAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
