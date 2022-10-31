#include "FastClassificationMC.h"
#include "Misc/FileHelper.h"
#include <limits>


UFastClassificationMC::UFastClassificationMC()
{
	
}

int UFastClassificationMC::CalcDistance(FVector A, FVector B) const
{
	return FMath::Floor(FVector::Distance(A,B));
}

uint32** UFastClassificationMC::DtwDistance(TArray<FVector> s, TArray<FVector> t)
{
	int n = s.Num();
	int m = t.Num();
	uint32** DTW;
	
	for (int i = 0; i < n; ++i)
	{
		DTW = new uint32*;
		for (int j = 0; j < m; ++j)
		{
			DTW[i][j] = std::numeric_limits<uint32>::max();
		}
	}

	DTW[0][0] = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			int cost = CalcDistance(s[i],t[j]);
			DTW[i][j] = cost + FMath::Min3(DTW[i-1][j],DTW[i][j-1],DTW[i-1][j-1]);
		}
	}
	
	return DTW;
}

bool UFastClassificationMC::WriteToFile(FString Filename, FString FilePath)
{
	UE_LOG(LogTemp,Display,TEXT("%s"),*FPaths::ProjectSavedDir());
	//FFileHelper::SaveStringArrayToFile();
	return false;
}
