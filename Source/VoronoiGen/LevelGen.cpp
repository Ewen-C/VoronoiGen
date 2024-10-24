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
	
    TArray<UE::Geometry::FIndex3i> MyTriangles = MyDelaunay.GetTriangles();

	for (int i = 0; i < MyTriangles.Num() - 1; i++)
	{
        DrawDebugLine( GetWorld(), FVector(MyPoints[MyTriangles[i].A], 0), FVector(MyPoints[MyTriangles[i].B], 0),
            FColor::Blue, true, -1, 0, 2 );
        DrawDebugLine( GetWorld(), FVector(MyPoints[MyTriangles[i].B], 0), FVector(MyPoints[MyTriangles[i].C], 0),
            FColor::Blue, true, -1, 0, 2 );
        DrawDebugLine( GetWorld(), FVector(MyPoints[MyTriangles[i].C], 0), FVector(MyPoints[MyTriangles[i].A], 0),
            FColor::Blue, true, -1, 0, 2 );
	}

	// Voronoi generation
	// Can't pass TArray<TArray<FVector2D>> to a function ¯\_(ツ)_/¯

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
	// Can't pass UE::Geometry::FDelaunay2 or UE::Geometry::FIndex3i to a function ¯\_(ツ)_/¯

	// Edges must not be from bounds to bounds
	// Edges must not be checked twice

	// Step 1 : Create Adjacency List

	AdjacencyList.Empty();

	for (const UE::Geometry::FIndex3i& Triangle : MyTriangles)
	{
		for (int i = 0; i < 3; i++)
		{
            int From = Triangle[i];
            int To = Triangle[(i + 1) % 3];
            float Length = FVector2D::Distance(MyPoints[From], MyPoints[To]);
			
            if (!AdjacencyList.Contains(From)) AdjacencyList.Add(From, TArray<FGraphEdge>());
            if (!AdjacencyList.Contains(To)) AdjacencyList.Add(To, TArray<FGraphEdge>());
			
            AdjacencyList[From].Add(FGraphEdge(From, To, Length));
            AdjacencyList[To].Add(FGraphEdge(To, From, Length));
		}
	}

	// Step 2 : Create Priority Queue
	
    if (AdjacencyList.Num() == 0) return;
    
    MinimumSpanningTree.Empty();
    TSet<int32> VisitedVertices; // Unique list of the points already checked
    TArray<FGraphEdge> PriorityQueue; // TArray with heap properties - binary tree always automatically sorted by shortest values

	// Add first point, add its edges to the priority queue
	int32 StartVertex = AdjacencyList.CreateConstIterator().Key(); 
    VisitedVertices.Add(StartVertex);

    for (const FGraphEdge& Edge : AdjacencyList[StartVertex])
		PriorityQueue.HeapPush(Edge, [](const FGraphEdge& A, const FGraphEdge& B) { return A.Length > B.Length; });
    
    // Step 3 : Prim's algorithm
	
    while (PriorityQueue.Num() > 0 && VisitedVertices.Num() < AdjacencyList.Num())
    {
        // Check all edges and takes the shortest one -> CurrentEdge grabs it then it's removed from PriorityQueue
        FGraphEdge CurrentEdge;
        PriorityQueue.HeapPop(CurrentEdge, [](const FGraphEdge& A, const FGraphEdge& B) { return A.Length > B.Length; });

    	// Skips visited points, add new ones 
        if (VisitedVertices.Contains(CurrentEdge.ToVertex)) continue; 
        MinimumSpanningTree.Add(CurrentEdge);
        VisitedVertices.Add(CurrentEdge.ToVertex);

    	// Add new edges to check, added in the shortest
        for (const FGraphEdge& NewEdges : AdjacencyList[CurrentEdge.ToVertex])
        {
            if (!VisitedVertices.Contains(NewEdges.ToVertex))
				PriorityQueue.HeapPush(NewEdges, [](const FGraphEdge& A, const FGraphEdge& B) { return A.Length > B.Length; });
        }
    }
	
    for (const FGraphEdge& Edge : MinimumSpanningTree)
    {
        DrawDebugLine( GetWorld(), FVector(MyPoints[Edge.FromVertex], 0), FVector(MyPoints[Edge.ToVertex], 0),
            FColor::Green, true, -1, 0, 16 );
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

