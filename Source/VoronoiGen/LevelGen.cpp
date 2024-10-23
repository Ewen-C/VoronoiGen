// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGen.h"


// Sets default values
ALevelGen::ALevelGen()
{
    // Create a root component (to get a transform !)
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ALevelGen::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Actor Location : %s"), *GetActorLocation().ToString());
	
	GenerateRandPoints(50, 2000);	
    DelaunayTriangulation(2000);
}

void ALevelGen::GenerateRandPoints(const int NumberOfPoints, const float SpacingSize)
{
	PointCloud.Empty();

	for(int i = 0; i < NumberOfPoints; i++)
	{
		float pointX = FMath::RandRange(0.f, SpacingSize);
		float pointY = FMath::RandRange(0.f, SpacingSize);

		PointCloud.Add(FVector2D(pointX, pointY));
	}

	for (const FVector2D Point : PointCloud)
	{
		DrawDebugPoint(GetWorld(), FVector(Point, 0.0f), 10.0f, FColor::Black, true);
	}
}

void ALevelGen::DelaunayTriangulation(const float SpacingSize)
{
	UE::Geometry::FDelaunay2 MyDelaunay;
	MyDelaunay.bAutomaticallyFixEdgesToDuplicateVertices = true;
	MyDelaunay.Triangulate(PointCloud);

	UE::Geometry::FAxisAlignedBox2d Bounds(FVector2D(0.f, 0.f), FVector2D(SpacingSize, SpacingSize));

	// TArray<TArray<FVector2D>> VoronoiCells = MyDelaunay.GetVoronoiCells(PointCloud, false);
	TArray<TArray<FVector2D>> VoronoiCells = MyDelaunay.GetVoronoiCells(PointCloud, true, Bounds, 0);

	// for (const TArray<FVector2D> VoronoiCell : VoronoiCells)
	// {
	// 	if(!VoronoiCell.Num()) continue;
	// 	UE_LOG(LogTemp, Warning, TEXT("Nb edges Voronoi : %d"), VoronoiCell.Num());
	// 	
	// 	DrawDebugLine(GetWorld(), FVector(VoronoiCell[VoronoiCell.Num() - 1], 10), FVector(VoronoiCell[0], 10),
	// 		FColor::Red, true, -1, 0, 4);
	// 	
	// 	for (int i = 0; i < VoronoiCell.Num() - 1; ++i)
	// 	{
	// 		DrawDebugLine(GetWorld(), FVector(VoronoiCell[i], 10), FVector(VoronoiCell[i+1], 10),
	// 			FColor::Red, true, -1, 0, 4);
	// 	}
	// }

	for (const TArray<FVector2D>& VoronoiCell : VoronoiCells)
	{
		for (int32 i = 0; i < VoronoiCell.Num(); ++i)
		{
			DrawDebugLine(GetWorld(), FVector(VoronoiCell[i], 0), FVector(VoronoiCell[(i+1) % VoronoiCell.Num()], 0),
				FColor::Red, true, -1, 0, 4);
		}
	}
	
}
