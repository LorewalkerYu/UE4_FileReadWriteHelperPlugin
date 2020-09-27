// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Struct/ImageColorBuffer/ImageColorBuffer.h"
#include "ImageFileLibrary.generated.h"

/**
 * 
 */
UCLASS()
class IMAGEFILEHELPER_API UImageFileLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintCallable, Category = "FileReadWriteHelpers|ImageFile|FunctionLibrary")
			static void SaveBufferToFile(const FString& FilePath, const FImageColorBuffer& Buffer);

		UFUNCTION(BlueprintCallable, Category = "FileReadWriteHelpers|ImageFile|FunctionLibrary")
			static UTexture2D* LoadBufferToTexture2D(const FImageColorBuffer& Buffer);

		UFUNCTION(BlueprintCallable, Category = "FileReadWriteHelpers|ImageFile|FunctionLibrary")
			static UTexture2D* LoadFileToTexture2D(const FString& Path);
};
