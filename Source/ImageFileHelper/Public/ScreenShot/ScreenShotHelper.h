// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/ImageColorBuffer/ImageColorBuffer.h"
#include "ScreenShotHelper.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScreenShotFinished, FImageColorBuffer, Image);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category = "FileReadWriteHelpers|ImageFile|ScreenShot")
class IMAGEFILEHELPER_API UScreenShotHelper : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "FileReadWriteHelpers|ImageFile|ScreenShot|Helper")
		void RequestToTakeScreenShot();
	UFUNCTION()
		void OnScreenShotReceived(int32 InSizeX, int32 InSizeY, const TArray<FColor>& InImageData);

protected:

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "FileReadWriteHelpers|ImageFile|ScreenShot|Helper")
		FOnScreenShotFinished OnScreenShotFinished;

};
