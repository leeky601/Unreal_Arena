// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ArGameMode.h"
#include "ArSurvivalGameMode.generated.h"

class AArEnemyCharacter;

UENUM(BlueprintType)
enum class EArSurvivalGameModeState : uint8
{
	WaitSpawnNewWaves,
	SpawningNewWaves,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FArEnemyWaveSpwanerInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr< AArEnemyCharacter > SoftEnemyClassToSpawn ;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1 ;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3 ;

};

USTRUCT(BlueprintType)
struct FArEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray< FArEnemyWaveSpwanerInfo > EnemyWaveSpwanerDefinitions ;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyThisWave = 1 ;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate, EArSurvivalGameModeState, InState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSequencePlayDelegate, bool, bIsPlaying);

UCLASS()
class ARENA_API AArSurvivalGameMode : public AArGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentSurvivalGameModeState(EArSurvivalGameModeState InState);
	bool HasDoneAllWaves();
	void PreLoadSoftClassToSpawn();
	FArEnemyWaveSpawnerTableRow* GetCurrentWaveTableRow();
	int32 TrySpawnEnemiesWave();
	bool ShouldKeepWaveSpawn();

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor) ;
	
	UPROPERTY()
	EArSurvivalGameModeState CurrentSurvivalState ;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLevelSequencePlayDelegate OnLevelSequencePlayDelegate ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesSpawn ;

	UPROPERTY()
	float TimePassedSinceStateChanged ;

	UPROPERTY()
	int32 TotlaSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnWaitingTime = 5.f ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnDelayTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitingTime = 5.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1 ;

	UPROPERTY()
	TMap< TSoftClassPtr< AArEnemyCharacter >, UClass* > PreLoadedEnemyClassMap ;

	UPROPERTY()
	TArray<AActor*> TargetPointArray ;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSummonEnemies(const TArray<AArEnemyCharacter*>& InSummonEnemies);
};
