// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "RSplineMeshComponent.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RESSPLINEMESHCOMP_API URSplineMeshComponent : public USplineComponent
{
	GENERATED_BODY()
	
	URSplineMeshComponent();

	UPROPERTY(EditAnywhere, Category = "RSplineMesh")
	UStaticMesh* SplineStaticMesh;

	UPROPERTY(Transient)
	TArray<USplineMeshComponent*> SplineMeshes;

	USplineMeshComponent* AddSplineMesh(UStaticMesh* SplineMesh, int32 StartPointIndex, int32 EndPointIndex, ESplineMeshAxis::Type ForwardAxis = ESplineMeshAxis::X, ECollisionEnabled::Type MeshCollision = ECollisionEnabled::NoCollision);

public:

	UFUNCTION(BlueprintCallable, Category = "RSplineMesh")
	void CreateSplineMeshV(FVector SplineStartLocation, FVector SplineEndLocation, float ChunkLength = 50.f, float ZOffset = 0.f, ESplineMeshAxis::Type ForwardAxis = ESplineMeshAxis::X, ECollisionEnabled::Type MeshCollision = ECollisionEnabled::NoCollision, bool IncludeLastChunk = true);

	
	UFUNCTION(BlueprintCallable, Category = "RSplineMesh")
	void CreateSplineMeshA(AActor* SplineStartActor, AActor* SplineEndActor, float ChunkLength = 50.f, float ZOffset = 0.f, ESplineMeshAxis::Type ForwardAxis = ESplineMeshAxis::X, ECollisionEnabled::Type MeshCollision = ECollisionEnabled::NoCollision, bool IncludeLastChunk = true);
	
	
};
