// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/ArPickUpBase.h"
#include "ArStoneBase.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class ARENA_API AArStoneBase : public AArPickUpBase
{
	GENERATED_BODY()
	
public:
	AArStoneBase();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pick Up Stone")
	UNiagaraComponent* StoneNiagaraComp;

	virtual void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
