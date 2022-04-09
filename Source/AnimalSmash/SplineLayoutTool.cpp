// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineLayoutTool.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASplineLayoutTool::ASplineLayoutTool()
{
	PrimaryActorTick.bCanEverTick = false;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	MeshAlongSpline = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshAlongSpline"));
	MeshAlongSpline->SetupAttachment(RootComponent);
	
}

void ASplineLayoutTool::BeginPlay()
{
	Super::BeginPlay();
}

void ASplineLayoutTool::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	MeshAlongSpline->SetStaticMesh(PlacedMesh);
	MeshAlongSpline->ClearInstances();

	if(MeshAlongSpline->GetStaticMesh()) {
		const FBox BoundingBox = PlacedMesh->GetBoundingBox();
		double v = BoundingBox.Max.X - BoundingBox.Min.X;
		Spacing = offset + v;

		const int NumberOfInstances = floor(Spline->GetSplineLength() / Spacing);

		for (int i = 0; i < NumberOfInstances; i++) {
			FTransform Start_T = FTransform(Spline->GetLocationAtDistanceAlongSpline(Spacing * i, ESplineCoordinateSpace::World));
			FTransform End_T = FTransform(Spline->GetLocationAtDistanceAlongSpline(Spacing * i+1, ESplineCoordinateSpace::World));

			Start_T.SetRotation(UKismetMathLibrary::MakeRotFromX(Start_T.GetLocation() - End_T.GetLocation()).Quaternion());
		
			MeshAlongSpline->AddInstance(Start_T);
		}
	}
}

void ASplineLayoutTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

