// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "DebugHelper.h"
AArEnemyCharacter::AArEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

UPawnCombatComponent* AArEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void AArEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitStartUpData();
}

void AArEnemyCharacter::InitStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]() {
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(ArAbilitySystemComponent);
				}
			}
		)
	);
}
