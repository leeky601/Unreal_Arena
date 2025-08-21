// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaTypes/ArenaEnumTypes.h"
#include "ArGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API AArGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AArGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EArGameDifficulty GameDifficulty ;

public:
	FORCEINLINE EArGameDifficulty GetGameDifficulty() const { return GameDifficulty;}
};
