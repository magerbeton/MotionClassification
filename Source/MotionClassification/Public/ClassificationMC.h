// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GestureStruct.h"
#include "2D_IntArrStruct.h"
#include "MotionControllerComponent.h"
#include "ClassificationMC.generated.h"

struct F2D_Int16ArrStruct;
/**
 * 
 */
UCLASS()
class MOTIONCLASSIFICATION_API UClassificationMC : public UMotionControllerComponent
{
	GENERATED_BODY()

public:

	UClassificationMC();

	UFUNCTION(BlueprintCallable, Category="Logging")
	void GetControllerInfo();

	UPROPERTY()
	FVector LastHandLocation;

	UPROPERTY()
	float Time;

	UPROPERTY()
	FVector LastInterpolatedLocation;

	UPROPERTY()
	int InterpolatedIndex;

	//----------------------------------------------
	// var declarations

	UPROPERTY()
	float TargetTime;

	UPROPERTY()
	int TargetSamples;

	UPROPERTY()
	float TargetSampleRate;

	UPROPERTY()
	TArray<FVector> InterpolatedLocs;
	
	UFUNCTION(BlueprintCallable, Category="Testing")
	void TestInterpolation(FVector CurrentHandLoc, FVector newLastHandLoc, float newTime, float newDeltaTime);

	//----------------------------------------------
	// Comparison
	
	float CompareGestures(FGestureStruct Sample, FGestureStruct Source);

	UPROPERTY()
	TArray<F2DIntArrStruct> DTW;

	/*UFUNCTION()
	int FindMinInUint16Arr(TArray<uint16>& Arr);
	*/

	UFUNCTION()
	int FindSmallest(TArray<int>& Arr);
	
	UFUNCTION()
	void PrintDtwMatrix();

	void TestUint16();

	TArray<int> FindDTWNeighbors(int x, int y);
	int FindMinNeighbor(TArray<int> InValues);
	
	UFUNCTION(BlueprintCallable)
	void TestTestFunc();
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GenerateTest(FVector DirectionVector, FVector SupportVector);

	UFUNCTION(BlueprintCallable)
	void CompareTwoGestures();
	
	
protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;
};
