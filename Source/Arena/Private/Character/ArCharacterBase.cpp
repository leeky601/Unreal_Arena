// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArCharacterBase.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "AbilitySystem/ArAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AArCharacterBase::AArCharacterBase()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->bReceivesDecals = false;

    ArAbilitySystemComponent = CreateDefaultSubobject<UArAbilitySystemComponent>(TEXT("ArAbilitySystemComponent"));

    ArAttributeSet = CreateDefaultSubobject<UArAttributeSet>(TEXT("ArAttributeSet"));

    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AArCharacterBase::GetAbilitySystemComponent() const
{
    return GetArAbilitySystemComponent();
}

UPawnCombatComponent* AArCharacterBase::GetPawnCombatComponent() const
{
    return nullptr;
}

UPawnUIComponent* AArCharacterBase::GetPawnUIComponent() const
{
    return nullptr;
}

void AArCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (ArAbilitySystemComponent)
    {
        ArAbilitySystemComponent->InitAbilityActorInfo(this, this);

        ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
    }
}



