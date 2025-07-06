// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaGameplayTags.h"
#include "DebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* TargetActor)
{
	if (OverlappedActors.Contains(TargetActor)) return;

	OverlappedActors.AddUnique(TargetActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{

	}

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = TargetActor;

	if (bIsValidBlock)
	{

	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
		ArenaGameplayTags::Shared_Event_MeleeHit,
		Data);
}
