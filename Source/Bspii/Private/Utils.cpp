#include "BspiiPrivatePCH.h"
#include "Utils.h"

Utils::Utils()
{
	World = NULL;
}

// Set the world (on fire :3c)
void Utils::SetWorld()
{
	FLevelEditorModule & LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	World = LevelEditorModule.GetFirstLevelEditor().Get()->GetWorld();
}

UWorld* Utils::GetWorld()
{
	return World;
}

void Utils::SpawnTestCubeBrush(FVector Location)
{
	if (World == NULL)
	{
		SetWorld();
	}

	FString Value, Class, Name;
	ABrush * Brush = World->SpawnActor<ABrush>();
	Brush->BrushType = Brush_Add;
	UModel* Model = new(Brush, NAME_None, RF_Transactional)UModel(FPostConstructInitializeProperties(), Brush, 1);

	Brush->SetActorLocation(Location);

	// Polys
	FPoly Poly1;
	Poly1.PolyFlags &= ~PF_NoImport;
	Poly1.iLink = 0;
	new(Poly1.Vertices) FVector(-100, -100, -100);
	new(Poly1.Vertices) FVector(-100, -100, 100);
	new(Poly1.Vertices) FVector(-100, 100, 100);
	new(Poly1.Vertices) FVector(-100, 100, -100);
	Poly1.TextureU = FVector(0, 1, 0);
	Poly1.TextureV = FVector(0, 0, -1);
	Poly1.Normal = FVector(-1, 0, 0);
	Poly1.Base = Poly1.GetMidPoint();
	Poly1.Material = NULL;
	Poly1.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly1);
	Poly1.GetMidPoint();

	FPoly Poly2;
	Poly2.PolyFlags &= ~PF_NoImport;
	Poly2.iLink = 1;
	new(Poly2.Vertices) FVector(-100, 100, -100);
	new(Poly2.Vertices) FVector(-100, 100, 100);
	new(Poly2.Vertices) FVector(100, 100, 100);
	new(Poly2.Vertices) FVector(100, 100, -100);
	Poly2.TextureU = FVector(1, 0, 0);
	Poly2.TextureV = FVector(0, 0, -1);
	Poly2.Normal = FVector(0, 1, 0);
	Poly2.Base = Poly2.GetMidPoint();
	Poly2.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly2);

	FPoly Poly3;
	Poly3.PolyFlags &= ~PF_NoImport;
	Poly3.iLink = 2;
	new(Poly3.Vertices) FVector(100, 100, -100);
	new(Poly3.Vertices) FVector(100, 100, 100);
	new(Poly3.Vertices) FVector(100, -100, 100);
	new(Poly3.Vertices) FVector(100, -100, -100);
	Poly3.TextureU = FVector(0, -1, 0);
	Poly3.TextureV = FVector(0, 0, -1);
	Poly3.Normal = FVector(1, 0, 0);
	Poly3.Base = Poly3.GetMidPoint();
	Poly3.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly3);

	FPoly Poly4;
	Poly4.PolyFlags &= ~PF_NoImport;
	Poly4.iLink = 3;
	new(Poly4.Vertices) FVector(100, -100, -100);
	new(Poly4.Vertices) FVector(100, -100, 100);
	new(Poly4.Vertices) FVector(-100, -100, 100);
	new(Poly4.Vertices) FVector(-100, -100, -100);
	Poly4.TextureU = FVector(-1, 0, 0);
	Poly4.TextureV = FVector(0, 0, -1);
	Poly4.Normal = FVector(0, -1, 0);
	Poly4.Base = Poly4.GetMidPoint();
	Poly4.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly4);

	FPoly Poly5;
	Poly5.PolyFlags &= ~PF_NoImport;
	Poly5.iLink = 4;
	new(Poly5.Vertices) FVector(-100, 100, 100);
	new(Poly5.Vertices) FVector(-100, -100, 100);
	new(Poly5.Vertices) FVector(100, -100, 100);
	new(Poly5.Vertices) FVector(100, 100, 100);
	Poly5.TextureU = FVector(1, 0, 0);
	Poly5.TextureV = FVector(0, 1, 0);
	Poly5.Normal = FVector(0, 0, 1);
	Poly5.Base = Poly5.GetMidPoint();
	Poly5.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly5);

	FPoly Poly6;
	Poly6.PolyFlags &= ~PF_NoImport;
	Poly6.iLink = 5;
	new(Poly6.Vertices) FVector(-100, -100, -100);
	new(Poly6.Vertices) FVector(-100, 100, -100);
	new(Poly6.Vertices) FVector(100, 100, -100);
	new(Poly6.Vertices) FVector(100, -100, -100);
	Poly6.TextureU = FVector(1, 0, 0);
	Poly6.TextureV = FVector(0, -1, 0);
	Poly6.Normal = FVector(0, 0, 1);
	Poly6.Base = Poly6.GetMidPoint();
	Poly6.Finalize(Brush, 0);
	new(Model->Polys->Element)FPoly(Poly6);

	Model->Linked = 1;
	Brush->BrushComponent->Mobility = EComponentMobility::Static;
	Model->Modify();
	Model->BuildBound();
	Brush->Brush->BuildBound();
	Brush->BrushComponent->Brush = Brush->Brush;
	Brush->PostEditImport();
	Brush->PostEditChange();
	Brush->RegisterAllComponents();
	Brush->EditorApplyRotation(FRotator(45, 45, 0), false, false, false);
}

PlaneData Utils::CalcPlaneData(BrushPlane inplane)
{
	PlaneData outPlane;

	FVector v1 = inplane.p1 - inplane.p2;
	FVector v2 = inplane.p3 - inplane.p2;
	outPlane.Normal = FVector::CrossProduct(v1, v2);
	outPlane.Normal.X = -outPlane.Normal.X; // Flip for Quake corods to UE's
	outPlane.Normal.Normalize();
	outPlane.dot = FVector::DotProduct(inplane.p1, outPlane.Normal);

	return outPlane;
}

float Utils::CalcCramerIntersection(int checkType, TArray<PlaneData>& Planes, int i, int j, int k)
{
	if (checkType == 1)
	{
		return Planes[i].Normal.X * Planes[j].Normal.Y * Planes[k].Normal.Z + Planes[i].Normal.Y * Planes[j].Normal.Z * Planes[k].Normal.X
			+ Planes[i].Normal.Z * Planes[j].Normal.X * Planes[k].Normal.Y - Planes[i].Normal.X * Planes[j].Normal.Z * Planes[k].Normal.Y
			- Planes[i].Normal.Y * Planes[j].Normal.X * Planes[k].Normal.Z - Planes[i].Normal.Z * Planes[j].Normal.Y * Planes[k].Normal.X;
	}
	else if (checkType == 2)
	{
		return Planes[i].dot * Planes[j].Normal.Y * Planes[k].Normal.Z + Planes[i].Normal.Y * Planes[j].Normal.Z * Planes[k].dot
			+ Planes[i].Normal.Z * Planes[j].dot * Planes[k].Normal.Y - Planes[i].dot * Planes[j].Normal.Z * Planes[k].Normal.Y
			- Planes[i].Normal.Y * Planes[j].dot * Planes[k].Normal.Z - Planes[i].Normal.Z * Planes[j].Normal.Y * Planes[k].dot;
	}
	else if (checkType == 3)
	{
		return Planes[i].Normal.X * Planes[j].dot * Planes[k].Normal.Z + Planes[i].dot * Planes[j].Normal.Z * Planes[k].Normal.X
			+ Planes[i].Normal.Z * Planes[j].Normal.X * Planes[k].dot - Planes[i].Normal.X * Planes[j].Normal.Z * Planes[k].dot
			- Planes[i].dot * Planes[j].Normal.X * Planes[k].Normal.Z - Planes[i].Normal.Z * Planes[j].dot * Planes[k].Normal.X;
	}
	else if (checkType == 4)
	{
		return Planes[i].Normal.X * Planes[j].Normal.Y * Planes[k].dot + Planes[i].Normal.Y * Planes[j].dot * Planes[k].Normal.X
			+ Planes[i].dot * Planes[j].Normal.X * Planes[k].Normal.Y - Planes[i].Normal.X * Planes[j].dot * Planes[k].Normal.Y
			- Planes[i].Normal.Y * Planes[j].Normal.X * Planes[k].dot - Planes[i].dot * Planes[j].Normal.Y * Planes[k].Normal.X;
	}
	else
		return -1;
}

bool Utils::IsPointInBrush(FVector point, TArray<PlaneData>& Planes)
{
	for (int i = 0; i < Planes.Num(); i++)
	{
		if (FVector::DotProduct(point, Planes[i].Normal) - Planes[i].dot < 0.01)
			return false;
	}
	return true;
}

bool Utils::IsPointInSet(FVector point, TArray<FVector>& Verts)
{
	for (int i = 0; i < Verts.Num(); i++)
	{
		if (FVector(Verts[i] - point).Size() < 0.01)
			return true;
	}
	return false;
}

// Surely UE4 does this but I couldn't find it :<
FVector Utils::CalcTangent(FVector Vector)
{
	float vec[3] = { Vector.X, Vector.Y, Vector.Z };
	int maxIdx = 0;
	int minIdx = 0;

	// Find largest and smallest componets
	for (int i = 0; i < 3; i++)
	{
		const float value = vec[i];
		if (FMath::Abs(value) > FMath::Abs(vec[maxIdx]))
			maxIdx = i;
		else if (FMath::Abs(value) < FMath::Abs(vec[minIdx]))
			minIdx = i;
	}

	vec[minIdx] = 1.0f;
	vec[maxIdx] = 0.0f;

	FVector binormal = FVector(vec[0], vec[1], vec[2]);
	float dot = FVector::DotProduct(binormal, Vector);

	if (FMath::Abs(dot) > 0.01)
	{
		binormal = (binormal - (Vector * dot));
		binormal.Normalize();
	}

	FVector tangent = FVector::CrossProduct(binormal, Vector);
	tangent.Normalize();

	return tangent;
};

Utils::~Utils()
{
	World = NULL;
}

