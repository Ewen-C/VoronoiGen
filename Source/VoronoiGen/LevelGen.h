// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Containers/Set.h"
#include "Math/UnrealMathUtility.h"
#include "LevelGen.generated.h"

USTRUCT()
struct FTriangle
{
    GENERATED_BODY()
    FVector Vertex1, Vertex2, Vertex3;
	
    FTriangle() : Vertex1(FVector::ZeroVector), Vertex2(FVector::ZeroVector), Vertex3(FVector::ZeroVector){}
    FTriangle(const FVector& a, const FVector& b, const FVector& c) : Vertex1(a), Vertex2(b), Vertex3(c) {}
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UFUNCTION()
		TArray<FVector> GenerateRandPoints(int NumberOfPoints, float SpacingSize);

    UFUNCTION()
		void DelaunayTriangulation(const TArray<FVector>& Points);

private:
    UFUNCTION()
		void DrawTriangle(const FTriangle& T);
};
