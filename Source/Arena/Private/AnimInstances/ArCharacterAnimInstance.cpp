// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/ArCharacterAnimInstance.h"
#include "Character/ArCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UArCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AArCharacterBase>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}

}

void UArCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningMovementComponent)
	{
		return;
	}
	
	GroundSpeed = OwningMovementComponent->Velocity.Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
