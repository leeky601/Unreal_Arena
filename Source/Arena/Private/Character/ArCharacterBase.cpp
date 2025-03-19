// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArCharacterBase.h"

// Sets default values
AArCharacterBase::AArCharacterBase()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->bReceivesDecals = false;

}



