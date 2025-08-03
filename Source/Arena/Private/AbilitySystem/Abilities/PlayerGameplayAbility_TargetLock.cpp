// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/ArPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/ArWidgetBase.h"
#include "Controller/ArPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "ArFunctionLibrary.h"
#include "ArenaGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DebugHelper.h"

void UPlayerGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement(); 
	InitMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetMappingContext();
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentTargetLockActor ||
		UArFunctionLibrary::NativeDoesActorHaveTag(CurrentTargetLockActor, ArenaGameplayTags::Shared_Status_Dead) ||
		UArFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), ArenaGameplayTags::Shared_Status_Dead))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!UArFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), ArenaGameplayTags::Player_Status_Blocking)
		&&
		!UArFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), ArenaGameplayTags::Player_Status_Rolling);

	if (bShouldOverrideRotation)
	{
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetPlayerCharacterFromActorInfo()->GetActorLocation(), CurrentTargetLockActor->GetActorLocation());

		LookAt -= FRotator(LookAtOffset, 0.f, 0.f);

		const FRotator CurrentControllerRot = GetPlayerControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControllerRot, LookAt, DeltaTime, TargetLockRotInterpSpeed);

		GetPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	} 
}

void UPlayerGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableTargetToLock();
	
	TArray<AActor*> TargetsOnLeft;
	TArray<AActor*> TargetsOnRight;
	AActor* NewTargetToLock = nullptr;

	DivideAvailableTargetLeftAndRight(TargetsOnLeft, TargetsOnRight);

	if (InSwitchDirectionTag == ArenaGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestAvailableActor(TargetsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestAvailableActor(TargetsOnRight);
	}


	if (NewTargetToLock)
	{
		CurrentTargetLockActor = NewTargetToLock;
	}
}

void UPlayerGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableTargetToLock();

	if (AvailableTargetsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentTargetLockActor = GetNearestAvailableActor(AvailableTargetsToLock);

	if (CurrentTargetLockActor)
	{
		DrawTargetLockWidget();

		/*SetTargetLockWidgetPosition();*/
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UPlayerGameplayAbility_TargetLock::GetAvailableTargetToLock()
{
	AvailableTargetsToLock.Empty();

	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation() + GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebugTrace ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		TraceHits,
		true
	);

	for (const FHitResult& HitResult : TraceHits)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor != GetPlayerCharacterFromActorInfo())
			{
				AvailableTargetsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UPlayerGameplayAbility_TargetLock::GetNearestAvailableActor(TArray<AActor*> InAvailableActor)
{
	float NearestTargetDistance;
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActor, NearestTargetDistance);
}

void UPlayerGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to Target Lock WidgetClass"));

		DrawnTargetLockWidget = CreateWidget<UArWidgetBase>(GetPlayerControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget);

		DrawnTargetLockWidget->AddToViewport();
	}
}

void UPlayerGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentTargetLockActor)
	{
		CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;

	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentTargetLockActor->GetActorLocation(),
		ScreenPosition,
		true
	);
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* SizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = SizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = SizeBox->GetHeightOverride();
				}
			}
		);
	}
	ScreenPosition -= TargetLockWidgetSize / 2.f;

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);

}

void UPlayerGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;

	GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockWalkSpeed;
}

void UPlayerGameplayAbility_TargetLock::InitMappingContext()
{
	ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(SubSystem);

	SubSystem->AddMappingContext(TargetLockMappingContext, 1);
}

void UPlayerGameplayAbility_TargetLock::DivideAvailableTargetLeftAndRight(TArray<AActor*>& OutTargetOnLeft, TArray<AActor*>& OutTargetOnRight)
{
	if (!CurrentTargetLockActor || AvailableTargetsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormal = (CurrentTargetLockActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableTarget : AvailableTargetsToLock)
	{
		if (!AvailableTarget || AvailableTarget == CurrentTargetLockActor) continue;

		const FVector PlayerToAvailableNormal = (AvailableTarget->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossProduct = FVector::CrossProduct(PlayerToCurrentNormal, PlayerToAvailableNormal);

		if (CrossProduct.Z > 0.f)
		{
			OutTargetOnRight.AddUnique(AvailableTarget);
		}
		else 
		{
			OutTargetOnLeft.AddUnique(AvailableTarget);
		}
	}
}

void UPlayerGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UPlayerGameplayAbility_TargetLock::CleanUp()
{
	AvailableTargetsToLock.Empty();

	CurrentTargetLockActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}
	
	DrawnTargetLockWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

void UPlayerGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UPlayerGameplayAbility_TargetLock::ResetMappingContext()
{	
	if (!GetPlayerControllerFromActorInfo())
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetPlayerControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(SubSystem);

	SubSystem->RemoveMappingContext(TargetLockMappingContext);
}
