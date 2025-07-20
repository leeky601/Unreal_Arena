// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExcuteOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);
/**
 * 
 */
UCLASS()
class ARENA_API UAbilityTask_ExcuteOnTick : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_ExcuteOnTick();

	UFUNCTION(BlueprintCallable, Category = "Arena|AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExcuteOnTick* ExcuteOnTick(UGameplayAbility* OwningAbility);
	
	//~Begin UGameplayTask Interface
	virtual void TickTask(float DeltaTime) override;
	//~End UGameplayTask Interface

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick ;
};
