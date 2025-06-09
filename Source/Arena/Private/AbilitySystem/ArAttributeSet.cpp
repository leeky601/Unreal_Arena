// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ArAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "ArFunctionLibrary.h"
#include "ArenaGameplayTags.h"

#include "DebugHelper.h"

UArAttributeSet::UArAttributeSet()
{
	InitMaxHealth(1.f);
	InitCurrentHealth(1.f);
	InitMaxRage(1.f);
	InitCurrentRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UArAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		const FString Msg = FString::Printf(TEXT("OldHealth: %f, DmageDone: %f, CurrentHealth: %f"), OldHealth, DamageDone, NewCurrentHealth);

		Debug::Print(Msg, FColor::Green);

		//ToDo : Notify Ui

		//ToDo : Handle Character Death

		if (NewCurrentHealth == 0.0f)
		{
			UArFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), ArenaGameplayTags::Shared_Status_Dead);
		}
	}
}
