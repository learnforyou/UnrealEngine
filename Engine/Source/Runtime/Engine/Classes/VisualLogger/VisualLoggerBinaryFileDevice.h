// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "VisualLogger/VisualLogger.h"

#if ENABLE_VISUAL_LOG
class FVisualLoggerBinaryFileDevice : public FVisualLogDevice
{
public:
	static FVisualLoggerBinaryFileDevice& Get()
	{
		static FVisualLoggerBinaryFileDevice GDevice;
		return GDevice;
	}

	FVisualLoggerBinaryFileDevice();
	virtual void Cleanup(bool bReleaseMemory = false) override;
	virtual void StartRecordingToFile(float TImeStamp) override;
	virtual void StopRecordingToFile(float TImeStamp) override;
	virtual void SetFileName(const FString& InFileName) override;
	virtual void Serialize(const class UObject* LogOwner, FName OwnerName, const FVisualLogEntry& LogEntry) override;

	struct FFrameCacheItem
	{
		FFrameCacheItem() {}
		FFrameCacheItem(FName InOwnerName, const FVisualLogEntry& LogEntry) : OwnerName(OwnerName), Entry(LogEntry) { }

		FName OwnerName;
		FVisualLogEntry Entry;
	};
protected:
	int32 bUseCompression : 1;
	float FrameCacheLenght;
	float StartRecordingTime;
	float LastLogTimeStamp;
	FArchive* FileArchive;
	FString TempFileName;
	FString FileName;
	TArray<FFrameCacheItem> FrameCache;
};
#endif
