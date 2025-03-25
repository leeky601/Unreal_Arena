// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/ArHeroAnimInstance.h"
#include "Character/ArPlayerCharacter.h"

void UArHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<AArPlayerCharacter>(OwningCharacter);
	}
}

void UArHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelax = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		
		bShouldEnterRelax = (IdleElapsedTime >= EnterRelaxThreshold);
	}
}
