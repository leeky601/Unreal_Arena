// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/ArAttributeSet.h"
#include "ArenaGameplayTags.h"

#include "DebugHelper.h"

struct FArDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	FArDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArAttributeSet, DamageTaken, Target, false)
	}
};

static const FArDamageCapture& GetArDamageCapture()
{
	static FArDamageCapture ArDamageCapture;
	return ArDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	/*Slow way do capture*/

	/*FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UArAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UArAttributeSet, AttackPower)
	);

	FGameplayEffectAttributeCaptureDefinition AttackPowerDefinition(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);*/

	//RelevantAttributesToCapture.Add(AttackPowerDefinition);

	RelevantAttributesToCapture.Add(GetArDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetArDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetArDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.SourceTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetArDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	/*Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);*/

	float WeaponBaseDamage = 0.f;
	int32 UsedLightAttackCombo = 0;
	int32 UsedHeavyAttackCombo = 0;

	for (TPair<FGameplayTag, float> TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(ArenaGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			WeaponBaseDamage = TagMagnitude.Value;
			/*Debug::Print(TEXT("WeaponBaseDamage"), WeaponBaseDamage);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(ArenaGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackCombo = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedLightAttackCombo"), UsedLightAttackCombo);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(ArenaGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackCombo = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedHeavyAttackCombo"), UsedHeavyAttackCombo);*/
		}
	}


	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetArDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	/*Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);*/

	if (UsedLightAttackCombo != 0)
	{
		const float IncreaseDamagePercentLight = (UsedLightAttackCombo - 1) * 0.05f + 1.f;

		WeaponBaseDamage *= IncreaseDamagePercentLight;

		/*Debug::Print(TEXT("IncreaseWeaponBaseDamageLight"), WeaponBaseDamage);*/
	}

	if (UsedHeavyAttackCombo != 0)
	{
		const float IncreaseDamagePercentHeavy = UsedHeavyAttackCombo * 0.15f + 1.f;

		WeaponBaseDamage *= IncreaseDamagePercentHeavy;

		/*Debug::Print(TEXT("IncreaseWeaponBaseDamageHeavy"), WeaponBaseDamage);*/
	}

	const float FinalDamage = WeaponBaseDamage * SourceAttackPower / TargetDefensePower;
	Debug::Print(TEXT("FinalDamage"), FinalDamage);

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(GetArDamageCapture().DamageTakenProperty, EGameplayModOp::Override, FinalDamage)
		);
	}
}
