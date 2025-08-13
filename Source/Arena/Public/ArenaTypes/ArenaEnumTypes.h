#pragma once

UENUM()
enum class EArConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EArValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EArSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EArCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EArCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Canceled
};