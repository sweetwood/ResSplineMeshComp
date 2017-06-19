// Fill out your copyright notice in the Description page of Project Settings.

#include "ResSplineMeshComp.h"
#include "RSplineMeshComponent.h"


URSplineMeshComponent::URSplineMeshComponent()
{
	static	ConstructorHelpers::FObjectFinder<UStaticMesh> DWSplineMesh(TEXT("/Game/Meshes/SM_ArrowPlane.SM_ArrowPlane"));
	SplineStaticMesh = DWSplineMesh.Object;
}


USplineMeshComponent * URSplineMeshComponent::AddSplineMesh(UStaticMesh * SplineMesh, int32 StartPointIndex, int32 EndPointIndex, ESplineMeshAxis::Type ForwardAxis, ECollisionEnabled::Type MeshCollision)
{
	USplineMeshComponent* LocalSplineMeshComp = NewObject<USplineMeshComponent>(this);
	LocalSplineMeshComp->SetMobility(EComponentMobility::Movable);
	LocalSplineMeshComp->SetStaticMesh(SplineMesh);
	LocalSplineMeshComp->SetForwardAxis(ForwardAxis);
	LocalSplineMeshComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	FVector LocalStartLocation;
	FVector LocalStartTangent;
	GetLocationAndTangentAtSplinePoint(StartPointIndex, LocalStartLocation, LocalStartTangent, ESplineCoordinateSpace::Local);

	FVector LocalEndLocation;
	FVector LocalEndTangent;
	GetLocationAndTangentAtSplinePoint(EndPointIndex, LocalEndLocation, LocalEndTangent, ESplineCoordinateSpace::Local);

	LocalSplineMeshComp->SetStartAndEnd(LocalStartLocation, LocalStartTangent, LocalEndLocation, LocalEndTangent);
	LocalSplineMeshComp->SetCollisionEnabled(MeshCollision);
	LocalSplineMeshComp->RegisterComponent();

	return LocalSplineMeshComp;
}


void URSplineMeshComponent::CreateSplineMeshV(FVector SplineStartLocation, FVector SplineEndLocation)
{
	if (SplineMeshes.Num() > 0)
	{
		for (int i = 0; i < SplineMeshes.Num(); i++)
		{
			SplineMeshes[i]->DestroyComponent();
		}

		SplineMeshes.Empty();
	}

	ClearSplinePoints();

	float Dividend = (SplineEndLocation - SplineStartLocation).Size();
	float Divisor = 50.f;																			// Never set the Divisor equal 0.f or universe will implode
	const float Quotient = Dividend / Divisor;														// <-|
	int32 LastIndex = (Quotient < 0.f ? -1 : 1) * FMath::FloorToInt(FMath::Abs(Quotient));			// <-| Basically the same code of  "Division (whole and remainder) Blueprint node
	float Remainder = FMath::Fmod(Dividend, Divisor);												// <-| (UKismetMathLibrary::FMod)
	float Offset = Remainder / LastIndex;

	for (int i = 0; i < LastIndex; i++)
	{
		FVector UnitDirection = (SplineEndLocation - SplineStartLocation).GetSafeNormal();
		FVector NewPointPosition = SplineStartLocation + (UnitDirection * (i * (Divisor + Offset)));

		AddSplinePointAtIndex(NewPointPosition, i, ESplineCoordinateSpace::World);

		if (i > 0)
		{
			int32 StartIndex = GetNumberOfSplinePoints() - 2;
			int32 EndIndex = GetNumberOfSplinePoints() - 1; // or StartIndex + 1
			SplineMeshes.Add(AddSplineMesh(SplineStaticMesh, StartIndex, EndIndex));
		}
	}
}


void URSplineMeshComponent::CreateSplineMeshA(AActor * SplineStartActor, AActor * SplineEndActor)
{
	CreateSplineMeshV(SplineStartActor->GetActorLocation(), SplineEndActor->GetActorLocation());
}
