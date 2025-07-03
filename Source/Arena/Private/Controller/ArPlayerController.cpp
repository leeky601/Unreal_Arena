// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ArPlayerController.h"

AArPlayerController::AArPlayerController()
{
    PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId AArPlayerController::GetGenericTeamId() const 
{
    return PlayerTeamID;
}
