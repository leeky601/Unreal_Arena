// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArenaTypes/ArenaEnumTypes.h"
#include "ArFunctionLibrary.generated.h"

class UArAbilitySystemComponent;
class UPawnCombatComponent;

/**
 * 
 */
UCLASS()
class ARENA_API UArFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UArAbilitySystemComponent* NativeGetArenaASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary")
	static void RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EArConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponent(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary", meta = (DisplayName = "Get Pawn CombatComponent", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponent(AActor* InActor, EArValidType& OutValidType);
};
