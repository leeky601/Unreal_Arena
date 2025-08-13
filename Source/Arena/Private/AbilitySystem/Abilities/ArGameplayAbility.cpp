// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArGameplayAbility.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArFunctionLibrary.h"
#include "ArenaGameplayTags.h"

void UArGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EArenaAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UArGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EArenaAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UArGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UArAbilitySystemComponent* UArGameplayAbility::GetArAbilitySystemComponentFromActorInfo() const
{
	return Cast<UArAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UArGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetArAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC); 
}

FActiveGameplayEffectHandle UArGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EArSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EArSuccessType::Successful : EArSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

void UArGameplayAbility::ApplyEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty()) return;

	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());

	for (const FHitResult& HitResult : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(HitResult.GetActor()))
		{
			if (UArFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectSpecHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);

				if (ActiveGameplayEffectSpecHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						ArenaGameplayTags::Shared_Event_HitReact,
						Data
					);
				}
			}
		}
	}
}
