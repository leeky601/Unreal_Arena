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

	UFUNCTION(BlueprintPure, Category = "Arena|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn) ;

	UFUNCTION(BlueprintPure, Category = "Arena|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableValueAtLevel(const FScalableFloat& InScalableFloat, float Inlevel = 1.f) ;

	UFUNCTION(BlueprintPure, Category = "Arena|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference) ;

	UFUNCTION(BlueprintPure, Category = "Arena|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender) ;

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTarget(APawn* InInstigator, APawn* InTarget, const FGameplayEffectSpecHandle& InSpecHandle) ;

	UFUNCTION(BlueprintCallable, Category = "Arena|FunctionLibrary", meta = (Latent, ExpandEnumAsExecs = "CountDownActionInput|CountDownActionOutput", WorldContext = "WorldContextObject", LatentInfo = "LatentActionInfo", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EArCountDownActionInput CountDownActionInput, UPARAM(DisplayName = "Output") EArCountDownActionOutput& CountDownActionOutput, FLatentActionInfo LatentActionInfo);

};
