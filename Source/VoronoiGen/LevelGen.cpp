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

	// Points setup
	
	GenerateRandPoints(NbPoints);

	// Delaunay triangulation
	
	UE::Geometry::FDelaunay2 MyDelaunay;
	MyDelaunay.bAutomaticallyFixEdgesToDuplicateVertices = true;
	MyDelaunay.Triangulate(MyPoints);
	
    TArray<UE::Geometry::FIndex3i> Triangles = MyDelaunay.GetTriangles();

	for (int i = 0; i < Triangles.Num() - 1; i++)
	{
        DrawDebugLine( GetWorld(), FVector(MyPoints[Triangles[i].A], 0), FVector(MyPoints[Triangles[i].B], 0),
            FColor::Blue, true, -1, 0, 2 );
        DrawDebugLine( GetWorld(), FVector(MyPoints[Triangles[i].B], 0), FVector(MyPoints[Triangles[i].C], 0),
            FColor::Blue, true, -1, 0, 2 );
        DrawDebugLine( GetWorld(), FVector(MyPoints[Triangles[i].C], 0), FVector(MyPoints[Triangles[i].A], 0),
            FColor::Blue, true, -1, 0, 2 );
	}

	// Voronoi generation - can't pass TArray<TArray<FVector2D>> to a function ¯\_(ツ)_/¯

	UE::Geometry::FAxisAlignedBox2d Bounds(FVector2D(0.f, 0.f), FVector2D(SpacingSize, SpacingSize));
	TArray<TArray<FVector2D>> VoronoiCells = MyDelaunay.GetVoronoiCells(MyPoints, true, Bounds, 0);

	for (const TArray<FVector2D>& VoronoiCell : VoronoiCells)
	{
		for (int32 i = 0; i < VoronoiCell.Num(); ++i)
		{
			DrawDebugLine(GetWorld(), FVector(VoronoiCell[i], 0), FVector(VoronoiCell[(i+1) % VoronoiCell.Num()], 0),
				FColor::Red, true, -1, 0, 4);
		}
	}


	// Minimum Spanning Tree construction

	// Edges must not be part of bounds
	// Edges must not be checked twice
	

	for(const TArray<FVector2D>& VoronoiCell : VoronoiCells)
	{

	}

	
	
}

void ALevelGen::GenerateRandPoints(const int NumberOfPoints)
{
	MyPoints.Empty();

	for(int i = 0; i < NumberOfPoints; i++)
	{
		float pointX = FMath::RandRange(0.f, SpacingSize);
		float pointY = FMath::RandRange(0.f, SpacingSize);

		MyPoints.Add(FVector2D(pointX, pointY));
	}

	for (const FVector2D Point : MyPoints)
	{
		DrawDebugPoint(GetWorld(), FVector(Point, 0.0f), 10.0f, FColor::Black, true);
	}
}

