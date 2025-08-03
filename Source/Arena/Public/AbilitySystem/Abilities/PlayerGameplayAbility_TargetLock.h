// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ArPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_TargetLock.generated.h"

class UArWidgetBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ARENA_API UPlayerGameplayAbility_TargetLock : public UArPlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime) ;

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void TryLockOnTarget();
	void GetAvailableTargetToLock();
	AActor* GetNearestAvailableActor(TArray<AActor*> InAvailableActor);
	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	void InitTargetLockMovement();
	void InitMappingContext();
	void DivideAvailableTargetLeftAndRight(TArray<AActor*>& OutTargetOnLeft, TArray<AActor*>& OutTargetOnRight);

	void CancelTargetLockAbility();
	void CleanUp();
	void ResetTargetLockMovement();
	void ResetMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(2000.f, 2000.f, 300.f) ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery> > BoxTraceChannel ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowDebugTrace = false ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UArWidgetBase> TargetLockWidgetClass ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockRotInterpSpeed = 5.f ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockWalkSpeed = 150.f ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	UInputMappingContext* TargetLockMappingContext ;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float LookAtOffset = 20.f;

	UPROPERTY()
	TArray<AActor*> AvailableTargetsToLock ;
	
	UPROPERTY()
	AActor* CurrentTargetLockActor ;

	UPROPERTY()
	UArWidgetBase* DrawnTargetLockWidget ;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed ;
};
