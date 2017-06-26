// Fill out your copyright notice in the Description page of Project Settings.

#include "RSplineMeshComponent.h"
#include "ResSplineMeshComp.h"


URSplineMeshComponent::URSplineMeshComponent()
{
	// We will use that as a default static mesh on start, you can modify it anywhere.
	static	ConstructorHelpers::FObjectFinder<UStaticMesh> DWSplineMesh(TEXT("/Game/Meshes/SM_ArrowPlane.SM_ArrowPlane"));
	SplineStaticMesh = DWSplineMesh.Object;		
}


USplineMeshComponent * URSplineMeshComponent::AddSplineMesh(UStaticMesh * SplineMesh, int32 StartPointIndex, int32 EndPointIndex, ESplineMeshAxis::Type ForwardAxis, ECollisionEnabled::Type MeshCollision)
{
	USplineMeshComponent* LocalSplineMeshComp = NewObject<USplineMeshComponent>(this);
	LocalSplineMeshComp->SetMobility(EComponentMobility::Movable);
	LocalSplineMeshComp->SetStaticMesh(SplineMesh);
	LocalSplineMeshComp->SetForwardAxis(ForwardAxis);
	LocalSplineMeshComp->SetCollisionEnabled(MeshCollision);
	LocalSplineMeshComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	FVector LocalStartLocation;
	FVector LocalStartTangent;
	GetLocationAndTangentAtSplinePoint(StartPointIndex, LocalStartLocation, LocalStartTangent, ESplineCoordinateSpace::Local);

	FVector LocalEndLocation;
	FVector LocalEndTangent;
	GetLocationAndTangentAtSplinePoint(EndPointIndex, LocalEndLocation, LocalEndTangent, ESplineCoordinateSpace::Local);

	LocalSplineMeshComp->SetStartAndEnd(LocalStartLocation, LocalStartTangent, LocalEndLocation, LocalEndTangent);
	
	LocalSplineMeshComp->RegisterComponent();

	return LocalSplineMeshComp;
}


void URSplineMeshComponent::CreateSplineMeshV(FVector SplineStartLocation, FVector SplineEndLocation, float ChunkLength/* = 50.f*/, float ZOffset/* = 0.f*/, ESplineMeshAxis::Type ForwardAxis/* = ESplineMeshAxis::X*/, ECollisionEnabled::Type MeshCollision/* = ECollisionEnabled::NoCollision*/, bool IncludeLastChunk/* = true*/)
{
	// If previously we have created SplineMesh components destroy all.
	if (SplineMeshes.Num() > 0)
	{
		for (int i = 0; i < SplineMeshes.Num(); i++)
		{
			SplineMeshes[i]->DestroyComponent();
		}

		SplineMeshes.Empty();																			// Probably useless but i prefer to be sure that the array is clean
	}

	ClearSplinePoints();																				// We remove all the Spline points to start add these from scratch

	int32 LastChunk = (IncludeLastChunk) ? 1 : 0;

	float Dividend = (SplineEndLocation - SplineStartLocation).Size();
	float Divisor = (ChunkLength == 0.f) ? 50.f : ChunkLength;											// Prevent universe implosion												
	
	const float Quotient = Dividend / Divisor;															// <-| Basically the same code of  "Division (whole and remainder)" Blueprint node
	int32 LastIndex = (Quotient < 0.f ? -1 : 1) * FMath::FloorToInt(FMath::Abs(Quotient)) + LastChunk;	// <-| 
	float Remainder = FMath::Fmod(Dividend, Divisor);													// <-| (UKismetMathLibrary::FMod)
	
	float Offset = Remainder / LastIndex;	
	
	for (int i = 0; i < LastIndex; i++)
	{
		FVector UnitDirection = (SplineEndLocation - SplineStartLocation).GetSafeNormal();
		FVector NewPointPosition = SplineStartLocation + (UnitDirection * (i * (Divisor + Offset)));
		NewPointPosition.Z += ZOffset;

		AddSplinePointAtIndex(NewPointPosition, i, ESplineCoordinateSpace::World);

		if (i > 0)
		{
			int32 StartIndex = GetNumberOfSplinePoints() - 2;
			int32 EndIndex = GetNumberOfSplinePoints() - 1;		// or StartIndex + 1
			SplineMeshes.Add(AddSplineMesh(SplineStaticMesh, StartIndex, EndIndex, ForwardAxis, MeshCollision));
		}
	}
}


void URSplineMeshComponent::CreateSplineMeshA(AActor * SplineStartActor, AActor * SplineEndActor, float ChunkLength/* = 50.f*/, float ZOffset/* = 0.f*/, ESplineMeshAxis::Type ForwardAxis/* = ESplineMeshAxis::X*/, ECollisionEnabled::Type MeshCollision/* = ECollisionEnabled::NoCollision*/, bool IncludeLastChunk/* = true*/)
{
	CreateSplineMeshV(SplineStartActor->GetActorLocation(), SplineEndActor->GetActorLocation(), ChunkLength, ZOffset, ForwardAxis, MeshCollision, IncludeLastChunk);
}
