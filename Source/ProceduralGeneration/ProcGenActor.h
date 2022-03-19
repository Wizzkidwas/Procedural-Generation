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
		// Internal name | Sets name to be used for the blueprint
		GRASS	UMETA(DisplayName = "Grasslands"),
		DESERT	UMETA(DisplayName = "Desert"),
		SNOW	UMETA(DisplayName = "Tundra"),
		WATER	UMETA(DisplayName = "Water"),
		ARID	UMETA(DisplayName = "Arid Desert"),
		RED		UMETA(DisplayName = "Red")
	};
}

UCLASS(meta = (BlueprintSpawnableComponent))	// Allows blueprint to call functions
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
	// Functions that set up the landscape
	void CreateVertices();
	void JoinVertices();
	void SetColours();
	void CombineInformation();
	void SetMaterial();

	// Allows biome type to be set in blueprints
	UFUNCTION(BluePrintCallable, Category = "Biomes")
		colours::Biome GetBiomeType() const;

	UFUNCTION(BluePrintCallable, Category = "Biomes")
		void SetBiomeType(colours::Biome biomeType);


private:
	// These are private because they shouldn't be modified at runtime, but can be edited in the editor before running the game
	// "VisibleAnywhere" can be seen but can't be changed in blueprint
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* actorMesh;

	// "EditAnywhere" can be changed in blueprint
	UPROPERTY(EditAnywhere)
		class UMaterial* meshMaterial;

	// Default Values
	UPROPERTY(EditAnywhere)
		int xSize = 100;
	UPROPERTY(EditAnywhere)
		int ySize = 100;
	UPROPERTY(EditAnywhere)
		int scale = 1000;
	UPROPERTY(EditAnywhere)
		UPerlinNoise_ActorComponent* Noise;	// Use this one to call from the Component

	// Gives biome type selection its own category in the blueprints
	UPROPERTY(EditAnywhere, Category = "Biome")
		TEnumAsByte<colours::Biome> BiomeType = colours::SNOW;	// Defaults to white landscape

	// Data for landscape, should not be modified at all outside of their functions
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColours;
};