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
	
	TArray<FVector> MyPoints = GenerateRandPoints(10, 2500);	
    DelaunayTriangulation(MyPoints);
}

TArray<FVector> ALevelGen::GenerateRandPoints(int NumberOfPoints, float SpacingSize)
{
	TArray<FVector> points;

	for(int i = 0; i < NumberOfPoints; i++)
	{
		float pointX = FMath::FRandRange(0., SpacingSize);
		float pointY = FMath::FRandRange(0., SpacingSize);

		points.Add(FVector(pointX, pointY, 50.));

		// Draw debug spheres at each point location
		DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(pointX, pointY, 50.),
			20.0f, 12, FColor::Red, false, 3600.f);
	}

	return points;
}

void ALevelGen::DelaunayTriangulation(const TArray<FVector>& MyPoints)
{
	if(MyPoints.Num() < 3) return;

	UE::Geometry::FDelaunay2 delaunay;
	
	for (int i = 0; i < MyPoints.Num(); ++i)
	{
		DrawTriangle(FTriangle(MyPoints[i], MyPoints[(i+1) % MyPoints.Num()], MyPoints[(i+2) % MyPoints.Num()]));
	}
}

void ALevelGen::DrawTriangle(const FTriangle& T)
{
    FVector SpawnLocationA = GetActorLocation() + T.Vertex1;
    FVector SpawnLocationB = GetActorLocation() + T.Vertex2;
    FVector SpawnLocationC = GetActorLocation() + T.Vertex3;

    DrawDebugLine(GetWorld(), SpawnLocationA, SpawnLocationB, FColor::Green, true, 3600.f, 0, 2.f);
    DrawDebugLine(GetWorld(), SpawnLocationB, SpawnLocationC, FColor::Green, true, 3600.f, 0, 2.f);
    DrawDebugLine(GetWorld(), SpawnLocationC, SpawnLocationA, FColor::Green, true, 3600.f, 0, 2.f);
}
