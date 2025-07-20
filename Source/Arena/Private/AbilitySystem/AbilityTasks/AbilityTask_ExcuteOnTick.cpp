// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTask_ExcuteOnTick.h"

UAbilityTask_ExcuteOnTick::UAbilityTask_ExcuteOnTick()
{
	bTickingTask = true;
}

UAbilityTask_ExcuteOnTick* UAbilityTask_ExcuteOnTick::ExcuteOnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_ExcuteOnTick* Node = NewAbilityTask<UAbilityTask_ExcuteOnTick>(OwningAbility);
	return Node;
}

void UAbilityTask_ExcuteOnTick::TickTask(float DeltaTime)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
