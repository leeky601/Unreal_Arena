// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Character/ArEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "ArFunctionLibrary.h"

#include "DebugHelper.h"

void AArSurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	EArGameDifficulty SavedDifficulty;

	if (UArFunctionLibrary::TryLoadSavedGameDifficulty(SavedDifficulty))
	{
		GameDifficulty = SavedDifficulty;
	}
}

void AArSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign Data Table in SurvivalGameMode"));

	TotalWavesSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::WaitSpawnNewWaves);

	PreLoadSoftClassToSpawn();
}

void AArSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSurvivalState == EArSurvivalGameModeState::WaitSpawnNewWaves)
	{
		TimePassedSinceStateChanged += DeltaTime;

		if (TimePassedSinceStateChanged >= SpawnWaitingTime)
		{
			TimePassedSinceStateChanged = 0.f;

			SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::SpawningNewWaves);
		}
	}

	if (CurrentSurvivalState == EArSurvivalGameModeState::SpawningNewWaves)
	{
		TimePassedSinceStateChanged += DeltaTime;

		if (TimePassedSinceStateChanged >= SpawnDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnEnemiesWave();

			TimePassedSinceStateChanged = 0.f;

			SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::InProgress);
		}
	}

	if (CurrentSurvivalState == EArSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStateChanged += DeltaTime;

		if (TimePassedSinceStateChanged >= WaveCompletedWaitingTime)
		{
			TimePassedSinceStateChanged = 0.f;

			CurrentWaveCount++;

			if (HasDoneAllWaves())
			{
				SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::WaitSpawnNewWaves);
				PreLoadSoftClassToSpawn();
			}
		}
	}
}

void AArSurvivalGameMode::SetCurrentSurvivalGameModeState(EArSurvivalGameModeState InState)
{
	CurrentSurvivalState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(InState);
}

bool AArSurvivalGameMode::HasDoneAllWaves()
{
	return CurrentWaveCount > TotalWavesSpawn;
}

void AArSurvivalGameMode::PreLoadSoftClassToSpawn()
{
	if (HasDoneAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();

	for (const FArEnemyWaveSpwanerInfo& SpawnerInfo : GetCurrentWaveTableRow()->EnemyWaveSpwanerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo,this]()
				{
					if (UClass* LoadedClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedClass);
					}
				}
			)
		);
	}
	
}

FArEnemyWaveSpawnerTableRow* AArSurvivalGameMode::GetCurrentWaveTableRow()
{
	FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FArEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FArEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(FoundRow, TEXT("Coundn't find Table Row in DataTable %s"), *EnemyWaveSpawnerDataTable->GetName());

	return FoundRow;
}

int32 AArSurvivalGameMode::TrySpawnEnemiesWave()
{
	if (TargetPointArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointArray);
	}

	checkf(!TargetPointArray.IsEmpty(), TEXT("Couldn't find TargetPoint In Level"));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	uint32 SpawnEnemiesCountThisTime = 0;

	for (const FArEnemyWaveSpwanerInfo& SpawnerInfo : GetCurrentWaveTableRow()->EnemyWaveSpwanerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandTargetPointIndex = FMath::RandRange(0, TargetPointArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointArray[RandTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotator = TargetPointArray[RandTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandLocation, 400.f);

			RandLocation += FVector(0.f, 0.f, 150.f);

			AArEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AArEnemyCharacter>(LoadedEnemyClass, RandLocation, SpawnRotator, SpawnParameters);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				SpawnEnemiesCountThisTime++;
				TotlaSpawnedEnemiesThisWaveCounter++;
			}

			if (!ShouldKeepWaveSpawn())
			{
				return SpawnEnemiesCountThisTime;
			}
		}
	}

	return SpawnEnemiesCountThisTime;
}

bool AArSurvivalGameMode::ShouldKeepWaveSpawn()
{
	return TotlaSpawnedEnemiesThisWaveCounter < GetCurrentWaveTableRow()->TotalEnemyThisWave;
}

void AArSurvivalGameMode::RegisterSummonEnemies(const TArray<AArEnemyCharacter*>& InSummonEnemies)
{
	for (AArEnemyCharacter* SummonEnemy : InSummonEnemies)
	{
		if (SummonEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SummonEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}

void AArSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;

	if (ShouldKeepWaveSpawn())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnEnemiesWave();
	}
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		TotlaSpawnedEnemiesThisWaveCounter = 0;
		
		SetCurrentSurvivalGameModeState(EArSurvivalGameModeState::WaveCompleted);
	}
}


