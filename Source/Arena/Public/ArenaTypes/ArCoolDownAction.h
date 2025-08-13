// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaEnumTypes.h"

class FArenaCoolDownAction : public FPendingLatentAction
{
public:
	FArenaCoolDownAction(float InTotalCountDownTime, float InUpdatedInterval, float& InOutRemainingTime, EArCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
		:bNeedToCancel(false),
		TotalCountDownTime(InTotalCountDownTime),
		UpdatedInterval(InUpdatedInterval),
		OutRemainingTime(InOutRemainingTime),
		CountDownOutput(InCountDownOutput),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallBackTarget(LatentInfo.CallbackTarget),
		ElapsedInterval(0.f),
		ElapsedTimeSinceStart(0.f)
	{
	}
	virtual void UpdateOperation(FLatentResponse& Response) override;

	void CancelAction();
private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdatedInterval;
	float& OutRemainingTime;
	EArCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallBackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;

};
