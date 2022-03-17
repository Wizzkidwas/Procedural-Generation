// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "PerlinNoise_ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ProcGenActor.generated.h"

// UE4's version of an enum that corresponds to biome type.
UENUM(BluePrintType)
namespace colours
{
	enum Biome
	{
		GRASS	UMETA(DisplayName = "Grasslands"),
		DESERT	UMETA(DisplayName = "Desert"),
		SNOW	UMETA(DisplayName = "Snowy")
	};
}

UCLASS(meta = (BlueprintSpawnableComponent))
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
	void CreateVertices();
	void JoinVertices();
	void CombineInformation();
	void SetMaterial();
	
	UFUNCTION(BluePrintCallable, Category = "Biomes")
		colours::Biome GetBiomeType() const;

	UFUNCTION(BluePrintCallable, Category = "Biomes")
		void SetBiomeType(colours::Biome biomeType);


private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* actorMesh;

	UPROPERTY(EditAnywhere)
		class UMaterial* meshMaterial;

	UPROPERTY(EditAnywhere)
		int xSize = 100;

	UPROPERTY(EditAnywhere)
		int ySize = 100;

	UPROPERTY(EditAnywhere)
		int scale = 1000;

	UPROPERTY(EditAnywhere)
		UPerlinNoise_ActorComponent* Noise;	// Use this one to call from the Component
	
	UPROPERTY(EditAnywhere, Category = "Biome")
		TEnumAsByte<colours::Biome> BiomeType = colours::SNOW;

	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColours;
};