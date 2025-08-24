// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArenaTypes/ArenaEnumTypes.h"
#include "ArSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API UArSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EArGameDifficulty SavedGameDifficulty ;
};
