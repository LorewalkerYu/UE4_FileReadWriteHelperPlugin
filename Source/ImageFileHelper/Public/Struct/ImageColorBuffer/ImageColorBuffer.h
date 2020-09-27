// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageColorBuffer.generated.h"
USTRUCT(BlueprintType, Blueprintable, Category = "FileReadWriteHelpers|ImageFile|Struct")
struct FImageColorBuffer
{
	GENERATED_USTRUCT_BODY()
public:
	FImageColorBuffer() {}
	FImageColorBuffer(int32 InSizeX, int32 InSizeY, const TArray<FColor>& InImageData) :SizeX(InSizeX), SizeY(InSizeY), ImageData(InImageData) {}
	int32 SizeX;
	int32 SizeY;
	TArray<FColor> ImageData;
};
