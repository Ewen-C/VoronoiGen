// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "CompGeom/Delaunay2.h"
#include "LevelGen.generated.h"

USTRUCT()
struct FGraphEdge
{
	GENERATED_BODY()
    int32 FromVertex;
    int32 ToVertex;
    float Length;
    
    bool operator < (const FGraphEdge& Other) const
    {
        return Length < Other.Length;
    }
};

UCLASS()
class VORONOIGEN_API ALevelGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY()
		USceneComponent* SceneComponent;

	UFUNCTION()
		void GenerateRandPoints(int NumberOfPoints);
	
	// Simple C++ classes doesn't have UFUNCTION limitations ! (⊙o⊙)
	TArray<TArray<FVector2D>> Test(int NumberOfPoints);
	UE::Geometry::FDelaunay2 Test2(int NumberOfPoints);

private:
    UPROPERTY(EditAnywhere, Category = "Generation")
		int NbPoints = 50;
	
    UPROPERTY(EditAnywhere, Category = "Generation")
		float SpacingSize = 2000;
	
	TArray<FVector2D> MyPoints;

    TMap<int32, TArray<FGraphEdge>> AdjacencyList;
    TArray<FGraphEdge> MinimumSpanningTree;
	
};
