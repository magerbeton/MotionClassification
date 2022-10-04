#pragma once

#include "CoreMinimal.h"
#include "2D_IntArrStruct.generated.h"


USTRUCT()
struct F2DIntArrStruct
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<int> A;

	F2DIntArrStruct(int Size = 0, bool FillFull = false)
	{
		A.Init(0,Size);
		/*
		if(FillFull)
		{
			A.Init(UINT16_MAX,Size);
		}
		else
		{
			A.Init(0,Size);
		}*/
	}
	
};