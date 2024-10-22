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
	
	const TArray<FVector2D> MyPoints = GenerateRandPoints(10, 2500);	
    DelaunayTriangulation(MyPoints);
}

TArray<FVector2D> ALevelGen::GenerateRandPoints(const int NumberOfPoints, const float SpacingSize)
{
	TArray<FVector2D> MyPoints;

	for(int i = 0; i < NumberOfPoints; i++)
	{
		float pointX = FMath::FRandRange(0., SpacingSize);
		float pointY = FMath::FRandRange(0., SpacingSize);

		MyPoints.Add(FVector2D(pointX, pointY));

		// Draw debug spheres at each point location
		DrawDebugPoint(GetWorld(), GetActorLocation() + FVector(pointX, pointY, 10.),
			20.0f, FColor::Black, true);
	}

	return MyPoints;
}

void ALevelGen::DelaunayTriangulation(const TArray<FVector2D>& MyPoints)
{
	if(MyPoints.Num() < 3) return;

	UE::Geometry::FDelaunay2 MyDelaunay;
	MyDelaunay.bAutomaticallyFixEdgesToDuplicateVertices = true;
	if( !MyDelaunay.Triangulate(MyPoints) ) return;

	TArray<TArray<FVector2D>> VoronoiCells = MyDelaunay.GetVoronoiCells(MyPoints, false);

	for (auto VoronoiCell : VoronoiCells)
	{
		if(!VoronoiCell.Num()) continue;
		UE_LOG(LogTemp, Warning, TEXT("Nb edges Voronoi : %d"), VoronoiCell.Num());
		
		DrawDebugLine(GetWorld(), FVector(VoronoiCell[VoronoiCell.Num() - 1], 10), FVector(VoronoiCell[0], 10),
			FColor::Red, true, -1, 0, 4);
		
		for (int i = 0; i < VoronoiCell.Num() - 1; ++i)
		{
			DrawDebugLine(GetWorld(), FVector(VoronoiCell[i], 10), FVector(VoronoiCell[i+1], 10),
				FColor::Red, true, -1, 0, 4);
		}
		
	}
	
}
