// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArCharacterBase.generated.h"

class UArAbilitySystemComponent;
class UArAttributeSet;

UCLASS()
class ARENA_API AArCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArCharacterBase();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface Interface
protected:

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UArAbilitySystemComponent* ArAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UArAttributeSet* ArAttributeSet;

	FORCEINLINE UArAbilitySystemComponent* GetArAbilitySystemComponent() const {return ArAbilitySystemComponent;}

	FORCEINLINE UArAttributeSet* GetArAttributeSet() const {return ArAttributeSet;}
};
