// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassificationMC.h"
#include "2D_IntArrStruct.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include <limits>


UClassificationMC::UClassificationMC()
{

	//----------------------------------------------
	// Default var inits

	LastHandLocation = FVector::ZeroVector;
	LastInterpolatedLocation = FVector::ZeroVector;
	

	
	TargetSamples = 60;
	TargetSampleRate = 1.0f/static_cast<float>(TargetSamples);
	TargetTime = 1.0f; // Time in seconds
	
	
	InterpolatedIndex = 0;
	InterpolatedLocs.Init(FVector::ZeroVector,TargetSamples);

	
	
}




void UClassificationMC::GetControllerInfo()
{
	FXRMotionControllerData MotionControllerDataR;
	UHeadMountedDisplayFunctionLibrary::GetMotionControllerData(this,EControllerHand::Right,MotionControllerDataR);

	UE_LOG(LogTemp,Display,TEXT("Hand R Trackingstate: %i"),static_cast<uint8>(MotionControllerDataR.TrackingStatus));
}

void UClassificationMC::TestInterpolation(FVector CurrentHandLoc, FVector newLastHandLoc, float newTime, float newDeltaTime)
{
	int TimeAdjuster = 0;
	const float FullTime = newTime+newDeltaTime;
	while(newTime+newDeltaTime>TargetSampleRate*TimeAdjuster)
	{
		float Percentage = newDeltaTime+(TargetSampleRate*TimeAdjuster)/FullTime;
		FMath::Lerp(GetComponentLocation(),newLastHandLoc,Percentage);
		TimeAdjuster++;
		UE_LOG(LogTemp,Display,TEXT("Percentage %f"),Percentage);
	}

	newTime = (newTime+newDeltaTime)-TargetSampleRate*TimeAdjuster;
	newLastHandLoc = GetComponentLocation();
}

float UClassificationMC::CompareGestures(FGestureStruct Sample, FGestureStruct Source)
{
	/*
	int DTWDistance(s: array [1..n], t: array [1..m]) {
	DTW := array [0..n, 0..m]
	
	for i := 0 to n
		for j := 0 to m
			DTW[i, j] := infinity
	DTW[0, 0] := 0
	
	for i := 1 to n
		for j := 1 to m
			cost := d(s[i], t[j])
			DTW[i, j] := cost + minimum(DTW[i-1, j  ],    // insertion
										DTW[i  , j-1],    // deletion
										DTW[i-1, j-1])    // match
	
	return DTW[n, m]
	}
	*/

	for (int i = 0; i<TargetSamples; i++)
	{
		DTW.Add(F2DIntArrStruct(TargetSamples,false));
		for(int n = 0; n<TargetSamples; n++)
		{
			DTW[i].A[n] = 65000;
		}
	}

	DTW[0].A[0] = 0;

	for (int i = 1; i<TargetSamples; i++)
	{
		for(int n = 1; n<TargetSamples; n++)
		{
			if(!Sample.VectorData.IsValidIndex(i))
			{
				return -1.0f;
			}
			if(!Source.VectorData.IsValidIndex(n))
			{
				return -1.0f;
			}
			const float cost = FVector::Distance(Sample.VectorData[i],Source.VectorData[n]);
			const int LowestNeighbor = FindMinNeighbor(FindDTWNeighbors(i,n));
			DTW[i].A[n] = cost+LowestNeighbor;
			
		}
	}

	TArray<FString> FileContent;
	for(int i = 0; i<TargetSamples; i++)
	{
		FString LineString;
		for(int n = 0; n<TargetSamples; n++)
		{
			LineString.Append(FString::Printf(TEXT("%i,"),DTW[i].A[n]));
		}
		FileContent.Add(LineString);
		//UE_LOG(LogTemp,Display,TEXT("%s"),*LineString);
	}

	FString Path = "C:\\Users\\User\\Documents\\Unreal Projects\\MotionClassification\\Data\\Hand_";
	Path.Append(MotionSource.ToString());
	Path.Append(".txt");
	FFileHelper::SaveStringArrayToFile(FileContent,*Path);
	
	
	return 0.0f;
}

/*
int UClassificationMC::FindMinInUint16Arr(TArray<uint16>& Arr)
{
	uint16 SmallestValue = 65000;
	for(int i = 0; i<Arr.Num(); i++)
	{
		if(Arr[i] < SmallestValue)
		{
			SmallestValue = Arr[i];
		}
	}
	return SmallestValue;
}*/

int UClassificationMC::FindSmallest(TArray<int>& Arr)
{
	int SmallestNumber = 65000;
	for(int i = 0; i<Arr.Num(); i++)
	{
		if(Arr[i] < SmallestNumber)
		{
			SmallestNumber = Arr[i];
		}
	}

	return SmallestNumber;
}

void UClassificationMC::PrintDtwMatrix()
{
	for(int i = 0; i<TargetSamples; i++)
	{
		FString RowString;
		for(int n = 0; i<TargetSamples; n++)
		{
			RowString.Append(FString::Printf(TEXT("%i,"),DTW[i].A[n]));
		}
		UE_LOG(LogTemp,Display,TEXT("%s"),*RowString);
	}
}

void UClassificationMC::TestUint16()
{
	const uint16 ANumberIsWow = 16;
	UE_LOG(LogTemp,Display,TEXT("uint16: %i"),ANumberIsWow);
}

TArray<int> UClassificationMC::FindDTWNeighbors(int x, int y)
{
	TArray<int> ReturnArray;
	TArray<FIntVector> Offsets = {FIntVector(-1,0,0),FIntVector(0,-1,0),FIntVector(-1,-1,0)};

	for(const FIntVector& Offset : Offsets)
	{
		if(DTW.IsValidIndex(x+Offset.X))
		{
			if(DTW.IsValidIndex(y+Offset.Y))
			{
				ReturnArray.Add(DTW[x+Offset.X].A[y+Offset.Y]);
			}
		}
	}
	
	return ReturnArray;
}

int UClassificationMC::FindMinNeighbor(TArray<int> InValues)
{
	if(!InValues.IsValidIndex(0))
	{
		return -1;
	}
	int SmallestValue = InValues[0];
	int IndexFoundAt = 0;
	for(int i = 1; i<InValues.Num(); i++)
	{
		if(!InValues.IsValidIndex(i))
		{
			return -1;
		}
		if(InValues[i]<SmallestValue)
		{
			SmallestValue = InValues[i];
			IndexFoundAt = i;
		}
	}
	return IndexFoundAt;
}

void UClassificationMC::TestTestFunc()
{
	TestUint16();
}

TArray<FVector> UClassificationMC::GenerateTest(FVector DirectionVector, FVector SupportVector)
{
	TArray<FVector> ReturnArray;
	FVector Direction;
	float Length;
	DirectionVector.ToDirectionAndLength(Direction,Length);

	for(int i = 0; i<TargetSamples; i++)
	{
		ReturnArray.Add(Direction*i+SupportVector);
	}
	return ReturnArray;
}

void UClassificationMC::CompareTwoGestures()
{
	const TArray<FVector> Arr1 = GenerateTest(FVector(1,1,1),FVector(2,3,4));
	const TArray<FVector> Arr2 = GenerateTest(FVector(1,1,3),FVector(2,3,4));

	const FGestureStruct Sample = FGestureStruct(Arr1, "Sample");
	const FGestureStruct Source = FGestureStruct(Arr2, "Source");
	const float Result = CompareGestures(Sample,Source);
	UE_LOG(LogTemp,Display,TEXT("Result = %f"),Result);

}

uint16** UClassificationMC::GenerateDtw(TArray<FVector> Input1, TArray<FVector> Input2)
{
	if(Input1.Num() != Input2.Num())
	{
		return {};
	}

	//setup the width and height of the matrix for dtw
	const int width = Input1.Num();
	const int height = Input1.Num();
	
	
	//init 2d array for the matrix
	uint16** array2d = new uint16*[width];

	for(int i = 0; i<width; i++)
	{
		array2d[i] = new uint16[height];
	}

	// fill the array with max values
	for(int i = 0; i<width; i++)
	{
		for(int n = 0; i<height; n++)
		{
			array2d[i][n] = std::numeric_limits<uint16>::max();
		}
	}

	array2d[0][0] = 0;

	for(int i = 1; i<width; i++)
	{
		for(int n = 1; n<height; n++)
		{
			const float cost= FVector::Distance(Input1[i],Input2[n]);
			array2d[i][n] = cost + FMath::Min3(array2d[i-1][n],array2d[i][n-1],array2d[i-1][n-1]);
		}
	}
	
	return array2d;
}

void UClassificationMC::TestUint16Implementation()
{
	const TArray<FVector> Arr1 = GenerateTest(FVector(1,1,1),FVector(2,3,4));
	const TArray<FVector> Arr2 = GenerateTest(FVector(1,1,3),FVector(2,3,4));

	uint16** DtwMatrix = GenerateDtw(Arr1,Arr2);
	for(int i = 0; i<Arr1.Num(); i++)
	{
		FString Line = "";
		for(int n = 0; n<Arr2.Num(); n++)
		{
			Line.Append(FString::Printf(TEXT("%i,"),DtwMatrix[i][n]));
		}
		UE_LOG(LogTemp,Display,TEXT("%s"),*Line);
	}
}

void UClassificationMC::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*int TimeAdjuster = 0;
	const float FullTime = Time+DeltaTime;
	while(Time+DeltaTime>TargetSampleRate*TimeAdjuster)
	{
		float Percentage = DeltaTime+(TargetSampleRate*TimeAdjuster)/FullTime;
		FVector InterpolatedLocation = FMath::Lerp(GetComponentLocation(),LastHandLocation,Percentage);

		FVector DeltaLoc = LastInterpolatedLocation-InterpolatedLocation;
		LastInterpolatedLocation = InterpolatedLocation;

		InterpolatedLocs[InterpolatedIndex] = DeltaLoc;
		InterpolatedIndex = (InterpolatedIndex + 1) % TargetSamples;
		UE_LOG(LogTemp,Display,TEXT("WriteIndex: %i"), InterpolatedIndex);
		TimeAdjuster++;
	}

	Time = (Time+DeltaTime)-TargetSampleRate*TimeAdjuster;
	LastHandLocation = GetComponentLocation();
	*/
}

void UClassificationMC::BeginPlay()
{
	Super::BeginPlay();
	CompareTwoGestures();
}
