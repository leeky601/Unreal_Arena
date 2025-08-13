// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUps/ArStoneBase.h"
#include "NiagaraComponent.h"
#include "Character/ArPlayerCharacter.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "ArenaGameplayTags.h"

AArStoneBase::AArStoneBase()
{
	StoneNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StoneNiagaraComp"));
	StoneNiagaraComp->SetupAttachment(GetRootComponent());
}

void AArStoneBase::OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AArPlayerCharacter* PlayerCharacter = Cast<AArPlayerCharacter>(OtherActor))
	{
		PlayerCharacter->GetArAbilitySystemComponent()->TryActivateAbilityByTag(ArenaGameplayTags::Player_Ability_PickUp_Stone);
	}
}
