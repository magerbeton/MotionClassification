// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassificationMC.h"

#include "HeadMountedDisplayFunctionLibrary.h"



UClassificationMC::UClassificationMC()
{

	//----------------------------------------------
	// Default var inits
	LastHandLocation = FVector::ZeroVector;

	TargetSamples = 60;
	TargetSampleRate = 1.0f/static_cast<float>(TargetSamples);

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

void UClassificationMC::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	int TimeAdjuster = 0;
	const float FullTime = Time+DeltaTime;
	while(Time+DeltaTime>TargetSampleRate*TimeAdjuster)
	{
		float Percentage = DeltaTime+(TargetSampleRate*TimeAdjuster)/FullTime;
		FMath::Lerp(GetComponentLocation(),LastHandLocation,Percentage);
		TimeAdjuster++;
	}

	Time = (Time+DeltaTime)-TargetSampleRate*TimeAdjuster;
	LastHandLocation = GetComponentLocation();
}
