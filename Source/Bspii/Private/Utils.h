#pragma once

#include "UnrealEd.h"
#include "LevelEditor.h"
#include "Runtime/Engine/Classes/Engine/Brush.h"
#include "Runtime/Engine/Classes/Engine/World.h"

struct Q1TexData
{
	float s;
	float t;
	float r;
	float s_scale;
	float t_scale;
};

struct BrushPlane
{
	FVector p1;
	FVector p2;
	FVector p3;
	FString Texture;
	Q1TexData Q1Tex;
};

struct PlaneData
{
	FVector Normal;
	float dot;
};

class Utils
{
public:
	Utils();
	~Utils();

	void SetWorld();
	UWorld* GetWorld();

	void SpawnTestCubeBrush(FVector Location);
	PlaneData CalcPlaneData(BrushPlane inplane);
	float CalcCramerIntersection(int checkType, TArray<PlaneData>& Planes, int i, int j, int k);
	FVector CalcTangent(FVector);

	static float GetAngleKey(const FVector vec, const FVector center, const FVector tan, const FVector binorm)
	{
		FVector r = vec - center;
		float y = FVector::DotProduct(r, tan);
		float x = FVector::DotProduct(r, binorm);
		return int(FMath::Atan2(y, x) / PI * 180.0);
	}

	inline static bool RelevantSortFunc(const FVector point1, const FVector point2)
	{
		return true;
	}

	bool IsPointInBrush(FVector point, TArray<PlaneData>& Planes);
	bool IsPointInSet(FVector point, TArray<FVector>& Verts);

private:
	UWorld* World;
};