// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API UBTService_OrientToTarget : public UBTService
{
	GENERATED_BODY()
	
	UBTService_OrientToTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey ;

	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed ;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};