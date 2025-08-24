// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "ArGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FArGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "GameData.Level")
	FGameplayTag LevelTag ;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;	

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};


UCLASS()
class ARENA_API UArGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
protected:
	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly)
	TArray<FArGameLevelSet> GameLevelSets ;

	virtual void OnPreLoadMap(const FString& MapName);

	virtual void OnPostLoadMap(UWorld* LoadedWorld);

public:
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InLevelTag) const;
};
