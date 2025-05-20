// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArCharacterBase.h"
#include "ArEnemyCharacter.generated.h"

class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class ARENA_API AArEnemyCharacter : public AArCharacterBase
{
	GENERATED_BODY()
	
public:
	AArEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UEnemyCombatComponent* EnemyCombatComponent;

private:
	void InitStartUpData();
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() {return EnemyCombatComponent;}
};
