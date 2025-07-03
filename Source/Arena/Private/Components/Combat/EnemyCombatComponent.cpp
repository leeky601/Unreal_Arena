// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"

#include "DebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* TargetActor)
{
	if (TargetActor) {
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("is hitting") + TargetActor->GetActorNameOrLabel());
	}
}
