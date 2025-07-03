// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ArBaseAnimInstance.h"
#include "ArFunctionLibrary.h"

bool UArBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UArFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}
