// Fill out your copyright notice in the Description page of Project Settings.
#include "ScreenShot/ScreenShotHelper.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

void UScreenShotHelper::RequestToTakeScreenShot()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine"));
		return;
	}
	GEngine->GameViewport->OnScreenshotCaptured().AddUObject(this, &UScreenShotHelper::OnScreenShotReceived);
	FScreenshotRequest::RequestScreenshot(false);
}

void UScreenShotHelper::OnScreenShotReceived(int32 InSizeX, int32 InSizeY, const TArray<FColor>& InImageData)
{
	FImageColorBuffer Buffer(InSizeX, InSizeY, InImageData);
	this->OnScreenShotFinished.Broadcast(Buffer);
}

