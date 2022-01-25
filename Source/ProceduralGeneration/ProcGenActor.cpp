// Fill out your copyright notice in the Description page of Project Settings.

#include <libnoise.h>
#include "ProcGenActor.h"

// using namespace noise;
// Constructor
AProcGenActor::AProcGenActor()
{
	actorMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = actorMesh;
	meshMaterial = CreateDefaultSubobject<UMaterial>(TEXT("MeshMaterial"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("'/Game/Materials/BlankMaterial'"));
	meshMaterial = Material.Object;
}

// Called when the actor is dropped into the world in the editor or at runtime
void AProcGenActor::PostActorCreated()
{
	Super::PostActorCreated();
	CreateLandscape();
}

// Called when actor is already in a level that is opened
void AProcGenActor::PostLoad()
{
	Super::PostLoad();
	CreateLandscape();
}

// Creates many triangles in a landscape style and places them into the mesh
void AProcGenActor::CreateLandscape()
{
	int sectionNumber = 0;
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			// Old height: FMath::RandRange(0, 50)
			// Creates vertex positions in local space
			double value1 = perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.01) * scale;
			double value2 = perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.02) * scale;
			double value3 = perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.03) * scale;
			double value4 = perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.04) * scale;

			vertices.Add(FVector(i * 100, j * 100, value1));
			vertices.Add(FVector(i * 100, 100 + (j * 100), value2));
			vertices.Add(FVector(100 + (i * 100), j * 100, value3));
			vertices.Add(FVector(100 + (i * 100), 100 + (j * 100), value4));

			// Adds the vertices to a triangle array to form a quad
			Triangles.Add(sectionNumber + 0);
			Triangles.Add(sectionNumber + 1);
			Triangles.Add(sectionNumber + 2);
			Triangles.Add(sectionNumber + 3);
			Triangles.Add(sectionNumber + 2);
			Triangles.Add(sectionNumber + 1);

			// Creates normals, tangents and sets up colours
			for (int k = 0; k < 4; k++)
			{
				normals.Add(FVector(0, 0, 1));
				tangents.Add(FProcMeshTangent(0, 0, -1));
				// vertexColours.Add(FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f));
				vertexColours.Add(FLinearColor(value1 / 10, value1 / 10, value1 / 10));
			}
			
			sectionNumber += 4;
			// Sets up UVs
			/*UV0.Add(FVector2D(0, 0));
			UV0.Add(FVector2D(1, 0));
			UV0.Add(FVector2D(0, 1));
			UV0.Add(FVector2D(1, 1));*/
		}
	}
	// Combines all the information and adds it to the mesh
	actorMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColours, tangents, true);
	// Enables collision
	actorMesh->ContainsPhysicsTriMeshData(true);
	SetMaterial();
	
	// Look for a way to make unreal calculate normals
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, Triangles, UV0, normals, tangents);

	// Large high res textures, tweak colours based on height
}

// Sets Material
void AProcGenActor::SetMaterial()
{
	// If it finds a valid material
	if (meshMaterial != nullptr)
	{
		auto MaterialInstance = UMaterialInstanceDynamic::Create(meshMaterial, this);
		actorMesh->SetMaterial(0, MaterialInstance);
	}
}
// Called when the game starts or when spawned
void AProcGenActor::BeginPlay()
{
	Super::BeginPlay();
	double value = perlinNoise.GetValue(1.25, 0.75, 0.50);
	UE_LOG(LogTemp, Warning, TEXT("Perlin hello world value: %f"), value);
}
