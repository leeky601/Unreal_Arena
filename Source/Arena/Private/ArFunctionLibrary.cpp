// Fill out your copyright notice in the Description page of Project Settings.


#include "ArFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UArAbilitySystemComponent* UArFunctionLibrary::NativeGetArenaASCFromActor(AActor* InActor)
{
    check(InActor);

    return CastChecked<UArAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UArFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UArFunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UArFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UArFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EArConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EArConfirmType::Yes : EArConfirmType::No;
}

UPawnCombatComponent* UArFunctionLibrary::NativeGetPawnCombatComponent(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent* UArFunctionLibrary::BP_GetPawnCombatComponent(AActor* InActor, EArValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponent(InActor);

    OutValidType = CombatComponent ? EArValidType::Valid : EArValidType::Invalid;

    return CombatComponent;
}
