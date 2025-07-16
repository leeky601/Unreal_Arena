// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_TargetLock.generated.h"

class UArWidgetBase;
/**
 * 
 */
UCLASS()
class ARENA_API UPlayerGameplayAbility_TargetLock : public UArPlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

private:
	void TryLockOnTarget();
	void GetAvailableTargetToLock();

	AActor* GetNearestAvailableActor();

	void DrawTargetLockWidget();

	void CancelTargetLockAbility();
	void CleanUp();

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(2000.f, 2000.f, 300.f) ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery> > BoxTraceChannel ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowDebugTrace = false ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UArWidgetBase> TargetLockWidgetClass ;

	UPROPERTY()
	TArray<AActor*> AvailableTargetsToLock ;
	
	UPROPERTY()
	AActor* TargetLockActor ;

	UPROPERTY()
	UArWidgetBase* DrawnTargetLockWidget ;
};
