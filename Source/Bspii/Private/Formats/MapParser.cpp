#include "../BspiiPrivatePCH.h"
#include "MapParser.h"
#include "CoreMisc.h"
#include <string>
#include <regex>
#include <stdlib.h> // ue4 equil?

using namespace std::tr1;

DECLARE_LOG_CATEGORY_EXTERN(BspiiLog, Log, All);
#define LOCTEXT_NAMESPACE "BspiiQuakeMapImporter"

MapParser::MapParser(Utils* utl)
{
	BspiiUtils = utl;
}

void MapParser::Load(FString path)
{
	GWarn->BeginSlowTask(LOCTEXT("StatusBeginImport", "Importing Quake 1 Level"), true, false);

	FString fileContent;
	FFileHelper::LoadFileToString(fileContent, *path);
	fileContent.ParseIntoArray(&Lines, TEXT("\n"), true);

	CurrentLineIdx = -1;
	while (GetNextline())
	{
		const FString str = CurrentLine;

		if (IsValidLine(str) && str.Contains(FString("{"), ESearchCase::IgnoreCase)) // Start of an entity
		{
			ReadEntity();
		}
		//UE_LOG(BspiiLog, Log, TEXT("%s"), *Lines[i]);
	}

	GWarn->EndSlowTask();
}

bool MapParser::GetNextline()
{
	CurrentLineIdx++;
	if (CurrentLineIdx < Lines.Num())
	{
		CurrentLine = Lines[CurrentLineIdx];
		return true;
	}

	return false;
}

bool MapParser::IsValidLine(FString line)
{
	if (line.StartsWith(FString("//"), ESearchCase::IgnoreCase))
		return false;
	else
		return true;
}

void MapParser::ReadEntity()
{
	UE_LOG(BspiiLog, Log, TEXT("-- Entity Start --"));

	while (GetNextline())
	{
		const FString str = CurrentLine;

		if (IsValidLine(str))
		{
			if (str.Contains(FString("\""), ESearchCase::IgnoreCase)) // Property
			{
				// TODO
				//UE_LOG(BspiiLog, Log, TEXT("  Property: %s"), *str);
			}
			else if (str.Contains(FString("{"), ESearchCase::IgnoreCase)) // Brush
			{
				ReadBrush();
			}
			else if (str.Contains(FString("}"), ESearchCase::IgnoreCase)) // End of Entity
			{
				UE_LOG(BspiiLog, Log, TEXT("-- Entity End --"));
				return;
			}
		}
	}
}

void MapParser::ReadBrush()
{
	UE_LOG(BspiiLog, Log, TEXT("-- Brush Start --"));

	TArray<BrushPlane> planes;

	while (GetNextline())
	{
		const FString str = CurrentLine;
		//UE_LOG(BspiiLog, Log, TEXT("str: %s"), *str);

		if (IsValidLine(str))
		{
			// A large part of this code is ported from https://github.com/inequation/map2t3d/blob/master/map2t3d.py
			// Big thanks to them for doing the heavy lifting when it comes to the planes intersections :>
			if (str.Contains(FString("}"), ESearchCase::CaseSensitive)) // End of Brush
			{
				UE_LOG(BspiiLog, Log, TEXT("%i"), planes.Num());
				UE_LOG(BspiiLog, Log, TEXT("-- Brush End --"));

				TArray<PlaneData> ProcessedPlanes;
				for (int i = 0; i < planes.Num(); i++)
				{
					ProcessedPlanes.Add(BspiiUtils->CalcPlaneData(planes[i]));
				}

				// Plane intersection to verts
				TArray<FVector> Verts;
				for (int i = 0; i < ProcessedPlanes.Num()-2; i++)
				{
					for (int j = 1; j < ProcessedPlanes.Num() - 1; j++)
					{
						for (int k = 2; k < ProcessedPlanes.Num(); k++)
						{
							if (i == j || j == k || i == k)
								continue;

							float a = BspiiUtils->CalcCramerIntersection(1, ProcessedPlanes, i, j, k);

							if (FMath::Abs(a) < 0.001)
								continue;

							float x = BspiiUtils->CalcCramerIntersection(2, ProcessedPlanes, i, j, k);
							float y = BspiiUtils->CalcCramerIntersection(3, ProcessedPlanes, i, j, k);
							float z = BspiiUtils->CalcCramerIntersection(4, ProcessedPlanes, i, j, k);

							FVector point = FVector(-x, y, z);
							point = point / a;

							if (/*BspiiUtils->IsPointInBrush(point, ProcessedPlanes) &&*/ !BspiiUtils->IsPointInSet(point, Verts))
							{
								Verts.Add(point);
							}
						}
					}
				}

				// Create the brush
				ABrush * Brush = BspiiUtils->GetWorld()->SpawnActor<ABrush>();
				Brush->BrushType = Brush_Add;
				UModel* Model = new(Brush, NAME_None, RF_Transactional)UModel(FPostConstructInitializeProperties(), Brush, 1);

				for (int i = 0; i < ProcessedPlanes.Num(); i++)
				{
					TArray<FVector> Relevant;
					FVector center = FVector::ZeroVector;

					// On same plane
					for (int aye = 0; aye < Verts.Num(); aye++)
					{
						if (FMath::Abs(FVector::DotProduct(Verts[aye], ProcessedPlanes[i].Normal) - ProcessedPlanes[i].dot) < 0.01)
						{
							center = center + Verts[aye];
							Relevant.Add(Verts[aye]);
						}
					}

					if (Relevant.Num() < 1)
						continue;

					center = center / Relevant.Num();
					FVector tangent = BspiiUtils->CalcTangent(center);
					FVector binormal = FVector::CrossProduct(center, tangent);
					//tangent.X = -tangent.X;
					//binormal.X = -binormal.X;
					binormal.Normalize();

					//Relevant.Sort(Utils::RelevantSortFunc);
					Relevant.Sort([&center, &tangent, &binormal](FVector point1, FVector point2)
					{
						float a1 = Utils::GetAngleKey(point1, center, tangent, binormal);
						float a2 = Utils::GetAngleKey(point2, center, tangent, binormal);
						UE_LOG(BspiiLog, Log, TEXT("Angle: %f"), a1);
						return (a1 > a2); // double check
					});

					const FVector norm = ProcessedPlanes[i].Normal;

					FPoly Poly;
					Poly.PolyFlags &= ~PF_NoImport;
					//Poly.iLink = 0;

					bool firstWritten = false;
					for (int eye = 0; eye < Relevant.Num(); eye++)
					{
						const FVector vec = Relevant[eye];

						//Poly.iLink = eye;
						Poly.Vertices.Add(vec);

						firstWritten = true;
					}

					if (Poly.Vertices.Num() >= 4)
					{
						Poly.TextureU = FVector(0, 1, 0);
						Poly.TextureV = FVector(0, 0, -1);
						Poly.Normal = norm;
						Poly.Base = center;
						Poly.Material = NULL;
						Poly.Finalize(Brush, 0);
						Model->Polys->Element.Add(Poly);
					}

					//UE_LOG(BspiiLog, Log, TEXT("CalcCramerIntersection: X:%f Y:%f Z:%f"), center.X, center.Y, center.Z);
				}

				Model->Linked = 1;
				Brush->BrushComponent->Mobility = EComponentMobility::Static;
				Model->Modify();
				Model->BuildBound();
				Brush->Brush->BuildBound();
				Brush->BrushComponent->Brush = Brush->Brush;
				Brush->PostEditImport();
				Brush->PostEditChange();
				Brush->RegisterAllComponents();
				//Brush->EditorApplyRotation(FRotator(45, 45, 0), false, false, false);

				return;
			}
			else // A brush plane
			{
				std::string seq = std::string(TCHAR_TO_ANSI(*str));
				smatch result;
				regex_search(seq, result, regex(Q3PLANEREGEX));

				BrushPlane plane;
				plane.p1 = FVector(atof(result[1].str().c_str()), atof(result[2].str().c_str()), atof(result[3].str().c_str()));
				plane.p2 = FVector(atof(result[4].str().c_str()), atof(result[5].str().c_str()), atof(result[6].str().c_str()));
				plane.p3 = FVector(atof(result[7].str().c_str()), atof(result[8].str().c_str()), atof(result[9].str().c_str()));
				/*plane.Texture = FString(result[10].str().c_str());

				plane.Q1Tex.s = atof(result[11].str().c_str());
				plane.Q1Tex.t = atof(result[12].str().c_str());
				plane.Q1Tex.r = atof(result[13].str().c_str());
				plane.Q1Tex.s_scale = atof(result[14].str().c_str());
				plane.Q1Tex.t_scale = atof(result[15].str().c_str());*/

				planes.Add(plane);

				/*UE_LOG(BspiiLog, Log, TEXT("Matchs: %i"), result.size());
				UE_LOG(BspiiLog, Log, TEXT("plane.Texture: %s"), *plane.Texture);

				for (size_t i = 0; i<result.size(); ++i)
				{
					UE_LOG(BspiiLog, Log, TEXT("Match:  %s"), *FString(result[i].str().c_str()));
				}*/
			}
		}
	}
}

MapParser::~MapParser()
{
	BspiiUtils = NULL;
}