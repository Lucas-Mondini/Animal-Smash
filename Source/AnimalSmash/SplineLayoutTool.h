// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineLayoutTool.generated.h"

UCLASS()
class ANIMALSMASH_API ASplineLayoutTool : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineLayoutTool();
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMesh* PlacedMesh;

	UPROPERTY(EditAnywhere)
	float offset;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(BlueprintReadWrite, Category="Spline")
	class USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite, Category="Spline, Mesh")
	UInstancedStaticMeshComponent* MeshAlongSpline;

	UPROPERTY(BlueprintReadOnly, Category="Spline")
	float Spacing;

public:	
	virtual void Tick(float DeltaTime) override;

};
