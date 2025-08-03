// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaGameplayTags.h"
#include "DebugHelper.h"
#include "Character/ArEnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "ArFunctionLibrary.h"
void UEnemyCombatComponent::OnHitTargetActor(AActor* TargetActor)
{
	if (OverlappedActors.Contains(TargetActor)) return;

	OverlappedActors.AddUnique(TargetActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UArFunctionLibrary::NativeDoesActorHaveTag(TargetActor, ArenaGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UArFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), ArenaGameplayTags::Enemy_Status_UnblockableAttack);

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UArFunctionLibrary::IsValidBlock(GetOwningPawn(), TargetActor);
	}

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = TargetActor;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TargetActor,
			ArenaGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
			ArenaGameplayTags::Shared_Event_MeleeHit,
			Data);
	}
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AArEnemyCharacter* EnemyCharacter = GetOwningPawn<AArEnemyCharacter>();

	UBoxComponent* LeftHandCollisionBox = EnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = EnemyCharacter->GetRightHandCollisionBox();

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
