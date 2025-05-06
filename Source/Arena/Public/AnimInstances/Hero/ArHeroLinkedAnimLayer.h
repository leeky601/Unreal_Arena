// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ArBaseAnimInstance.h"
#include "ArHeroLinkedAnimLayer.generated.h"


class UArHeroAnimInstance;
/**
 * 
 */
UCLASS()
class ARENA_API UArHeroLinkedAnimLayer : public UArBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UArHeroAnimInstance* GetPlayerAnimInstance() const;
};
