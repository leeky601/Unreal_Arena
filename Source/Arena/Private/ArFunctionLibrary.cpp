// Fill out your copyright notice in the Description page of Project Settings.


#include "ArFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArenaGameplayTags.h"
#include "ArenaTypes/ArCoolDownAction.h"
#include "ArGameInstance.h"
#include "SaveGame/ArSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "DebugHelper.h"
UArAbilitySystemComponent* UArFunctionLibrary::NativeGetArenaASCFromActor(AActor* InActor)
{
    check(InActor);

    return CastChecked<UArAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UArFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UArFunctionLibrary::RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UArFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UArAbilitySystemComponent* ASC = NativeGetArenaASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UArFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EArConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EArConfirmType::Yes : EArConfirmType::No;
}

UPawnCombatComponent* UArFunctionLibrary::NativeGetPawnCombatComponent(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent* UArFunctionLibrary::BP_GetPawnCombatComponent(AActor* InActor, EArValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponent(InActor);

    OutValidType = CombatComponent ? EArValidType::Valid : EArValidType::Invalid;

    return CombatComponent;
}

bool UArFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

float UArFunctionLibrary::GetScalableValueAtLevel(const FScalableFloat& InScalableFloat, float Inlevel)
{
    return InScalableFloat.GetValueAtLevel(Inlevel);
}

FGameplayTag UArFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    check(InAttacker && InVictim);
    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalize = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalize);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalize);

    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
    {
        return ArenaGameplayTags::Shared_Status_HitReact_Front;
    }
    if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
    {
        return ArenaGameplayTags::Shared_Status_HitReact_Left;
    }
    if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
    {
        return ArenaGameplayTags::Shared_Status_HitReact_Right;
    }
    if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
    {
        return ArenaGameplayTags::Shared_Status_HitReact_Back;
    }

    return ArenaGameplayTags::Shared_Status_HitReact_Front;
}

bool UArFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
    
    /*const FString DebugString = FString::Printf(TEXT("Dotproduct: %f %s"), DotResult, DotResult < -0.2f ? TEXT("Block is Valid") : TEXT("Block is not Valid"));

    Debug::Print(DebugString, DotResult < -0.2f ? FColor::Green : FColor::Red);*/

    return DotResult < -0.2f;
}

bool UArFunctionLibrary::ApplyGameplayEffectSpecHandleToTarget(APawn* InInstigator, APawn* InTarget, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UArAbilitySystemComponent* SourceASC = NativeGetArenaASCFromActor(InInstigator);
    UArAbilitySystemComponent* TargetASC = NativeGetArenaASCFromActor(InTarget);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UArFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EArCountDownActionInput CountDownActionInput, UPARAM(DisplayName = "Output") EArCountDownActionOutput& CountDownActionOutput, FLatentActionInfo LatentActionInfo)
{
    UWorld* World = nullptr;
    if (GEngine)
    {
        World= GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World) return;

    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

    FArenaCoolDownAction* FoundAction = LatentActionManager.FindExistingAction<FArenaCoolDownAction>(LatentActionInfo.CallbackTarget, LatentActionInfo.UUID);

    if (CountDownActionInput == EArCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            LatentActionManager.AddNewAction(
                LatentActionInfo.CallbackTarget,
                LatentActionInfo.UUID,
                new FArenaCoolDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownActionOutput, LatentActionInfo)
                );
        }
    }

    if (CountDownActionInput == EArCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            FoundAction->CancelAction();
        }
    }
}

UArGameInstance* UArFunctionLibrary::GetArGameInstance(UObject* WorldContextObject)
{
    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            return World->GetGameInstance<UArGameInstance>();
        }
    }

    return nullptr;
}

void UArFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EArGameInputMode InInputMode)
{
    APlayerController* PlayerController = nullptr;

    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            PlayerController = World->GetFirstPlayerController();
        }
    }

    FInputModeGameOnly GameOnly;
    FInputModeUIOnly UIOnly;

    switch (InInputMode)
    {
    case EArGameInputMode::GameOnly:
        PlayerController->SetInputMode(GameOnly);
        PlayerController->bShowMouseCursor = false;

        break;
    case EArGameInputMode::UIOnly:
        PlayerController->SetInputMode(UIOnly);
        PlayerController->bShowMouseCursor = true;

        break;
    default:
        break;
    }
}

void UArFunctionLibrary::SaveGameDifficulty(EArGameDifficulty InGameDifficulty)
{
    USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UArSaveGame::StaticClass());

    if (UArSaveGame* ArSaveGameObject = Cast<UArSaveGame>(SaveGameObject))
    {
        ArSaveGameObject->SavedGameDifficulty = InGameDifficulty;

        const bool bWasSaved = UGameplayStatics::SaveGameToSlot(ArSaveGameObject, ArenaGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

        Debug::Print(bWasSaved ? TEXT("Save Sucessed") : TEXT("Save Failed"));
    }
}

bool UArFunctionLibrary::TryLoadSavedGameDifficulty(EArGameDifficulty& OutGameDifficulty)
{
    if (UGameplayStatics::DoesSaveGameExist(ArenaGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
    {
        USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(ArenaGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

        if (UArSaveGame* ArSaveGameObject = Cast<UArSaveGame>(SaveGameObject))
        {
            OutGameDifficulty = ArSaveGameObject->SavedGameDifficulty;

            Debug::Print(TEXT("Load Sucessed"));

            return true;
        }
    }

    return false;
}
