// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaTypes/ArCoolDownAction.h"

void FArenaCoolDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EArCountDownActionOutput::Canceled;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallBackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EArCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallBackTarget);

		return;
	}

	if (ElapsedInterval < UpdatedInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdatedInterval > 0.f ? UpdatedInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EArCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink, CallBackTarget);

		ElapsedInterval = 0.f;
	}
}

void FArenaCoolDownAction::CancelAction()
{
	bNeedToCancel = true;
}
