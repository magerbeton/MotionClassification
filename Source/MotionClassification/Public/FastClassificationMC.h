#pragma once

#include "MotionControllerComponent.h"
#include "FastClassificationMC.generated.h"

UCLASS()
class MOTIONCLASSIFICATION_API UFastClassificationMC : public UMotionControllerComponent
{
	GENERATED_BODY()

public:

	UFastClassificationMC();
	
	int CalcDistance(FVector A, FVector B) const;

	uint32** DtwDistance(TArray<FVector> s, TArray<FVector> t);
	
	bool WriteToFile(FString Filename, FString FilePath);
};
