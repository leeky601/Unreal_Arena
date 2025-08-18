// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_PickUp.generated.h"

class AArStoneBase;
/**
 * 
 */
UCLASS()
class ARENA_API UPlayerGameplayAbility_PickUp : public UArPlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	UFUNCTION(BlueprintCallable)
	void CollectStones() ;

	UFUNCTION(BlueprintCallable)
	void ConsumeStones() ;

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f ;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f) ;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > ObjectTypes ;

	UPROPERTY(EditDefaultsOnly)
	bool bShowDebugTrace;

	TArray< AArStoneBase*> CollectedStones;
};
