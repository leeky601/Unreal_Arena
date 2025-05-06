// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/ArHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/ArHeroAnimInstance.h"

UArHeroAnimInstance* UArHeroLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UArHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
