// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/ArPickUpBase.h"
#include "ArStoneBase.generated.h"

class UNiagaraComponent;
class UArAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class ARENA_API AArStoneBase : public AArPickUpBase
{
	GENERATED_BODY()
	
public:
	AArStoneBase();

	void Consume(UArAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed() ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pick Up Stone")
	UNiagaraComponent* StoneNiagaraComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass ;

	virtual void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
