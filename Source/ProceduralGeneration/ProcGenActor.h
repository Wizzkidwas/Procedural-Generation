// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PerlinNoise_ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ProcGenActor.generated.h"

UCLASS()
class PROCEDURALGENERATION_API AProcGenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcGenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

public:	
	void CreateLandscape();
	void SetMaterial();

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* actorMesh;

	UPROPERTY(EditAnywhere)
		class UMaterial* meshMaterial;
	
	UPROPERTY(VisibleAnywhere)
		int x = 50;
	
	UPROPERTY(VisibleAnywhere)
		int y = 50;

	UPerlinNoise_ActorComponent* Noise;
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
};
