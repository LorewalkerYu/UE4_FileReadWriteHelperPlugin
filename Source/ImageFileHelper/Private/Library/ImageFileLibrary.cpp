// Fill out your copyright notice in the Description page of Project Settings.

#include "Library/ImageFileLibrary.h"
#include "Misc/FileHelper.h"
#include "Engine/Texture2D.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

void UImageFileLibrary::SaveBufferToFile(const FString& FilePath, const FImageColorBuffer& Buffer)
{
	// Load image wrapper module
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

	// Provide image wrapper with raw pixels (TArray<FColor>)
	ImageWrapper->SetRaw(&Buffer.ImageData[0], Buffer.ImageData.Num() * sizeof(FColor), Buffer.SizeX, Buffer.SizeY, ERGBFormat::BGRA, 8);

	// Get the image using 90% JPEG compression
	const TArray64<uint8> CompressedImage = ImageWrapper->GetCompressed(90);
	FFileHelper::SaveArrayToFile(CompressedImage, *FilePath);
}

UTexture2D* UImageFileLibrary::LoadBufferToTexture2D(const FImageColorBuffer& Buffer)
{
	UTexture2D* LoadedT2D = UTexture2D::CreateTransient(Buffer.SizeX, Buffer.SizeY, PF_B8G8R8A8);
	//Copy!
	void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Buffer.ImageData.GetData(), Buffer.ImageData.Num());
	LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
	LoadedT2D->UpdateResource();
	return LoadedT2D;
}

UTexture2D* UImageFileLibrary::LoadFileToTexture2D(const FString& Path)
{
	FString FormatStr = Path.Replace(TEXT("\""), TEXT(""));
	FormatStr = FormatStr.Right(4);
	EImageFormat ImageFormat = EImageFormat::Invalid;
	ERGBFormat RGBFormat     = ERGBFormat::Invalid;

	if (FormatStr.Contains(".jpg"))
	{
		ImageFormat = EImageFormat::JPEG;
		RGBFormat   = ERGBFormat::BGRA;
	}
	else if (FormatStr.Contains(".png"))
	{
		ImageFormat = EImageFormat::PNG;
		RGBFormat   = ERGBFormat::RGBA;
	}

	if (ImageFormat == EImageFormat::Invalid)
	{
		UE_LOG(LogTemp, Error, TEXT("Format Invalid"))
		return nullptr;
	}

	TArray<uint8, FDefaultAllocator64> RawData;
	if (!FFileHelper::LoadFileToArray(RawData, *Path))
	{
		UE_LOG(LogTemp, Error, TEXT("FileLoading Failed"))
		return nullptr;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper  = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	UTexture2D* LoadedT2D = nullptr;

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawData.GetData(), RawData.Num()))
	{
		TArray64<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!LoadedT2D)
			{
				UE_LOG(LogTemp, Error, TEXT("Create Texture Failed"))
				return nullptr;
			}

			//Out!
			int32 Width = ImageWrapper->GetWidth();
			int32 Height = ImageWrapper->GetHeight();

			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			//Update!
			LoadedT2D->UpdateResource();
		}
	}
	return LoadedT2D;
}
