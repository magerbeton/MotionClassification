// Copyright Konstantin Passig

#pragma once

#include "CoreMinimal.h"
#include "GestureStruct.generated.h"

USTRUCT(BlueprintType)
struct FGestureStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> VectorData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString GestureName;

	FGestureStruct(TArray<FVector> newVectorData = {}, FString newGestureName = "None")
	{
		VectorData = newVectorData;
		GestureName = newGestureName;
	}
};