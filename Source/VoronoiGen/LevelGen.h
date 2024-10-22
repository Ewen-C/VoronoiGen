// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "CompGeom/Delaunay2.h"
#include "LevelGen.generated.h"

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
		TArray<FVector2D> GenerateRandPoints(int NumberOfPoints, float SpacingSize);

    UFUNCTION()
		void DelaunayTriangulation(const TArray<FVector2D>& Points);
};
