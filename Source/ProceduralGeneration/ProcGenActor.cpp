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
	Noise = CreateDefaultSubobject< UPerlinNoise_ActorComponent>(TEXT("Noise"));
}

// Called when the actor is dropped into the world in the editor or at runtime
void AProcGenActor::PostActorCreated()
{
	Super::PostActorCreated();
	Noise->SetupOptions(Noise->GetFrequency(), Noise->GetLacunarity(), Noise->GetNoiseQuality(), Noise->GetOctaveCount(), Noise->GetPersistence(), Noise->GetSeed());
	CreateVertices();
	JoinVertices();
	CombineInformation();
	SetMaterial();
}

// Called when actor is already in a level that is opened
void AProcGenActor::PostLoad()
{
	Super::PostLoad();
	// CreateLandscape();
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
			double value1 = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.01) * scale;
			double value2 = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.02) * scale;
			double value3 = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.03) * scale;
			double value4 = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, sectionNumber + 0.04) * scale;

			// Create one per loop and connect in a later loop
			vertices.Add(FVector(i * 100, j * 100, value1));
			vertices.Add(FVector(i * 100, 100 + (j * 100), value2));
			vertices.Add(FVector(100 + (i * 100), j * 100, value3));
			vertices.Add(FVector(100 + (i * 100), 100 + (j * 100), value4));

			// Adds the vertices to a triangle array to form a quad
			// This section in another loop afterwards, joining the individual vertices
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

// Sets up vertices
void AProcGenActor::CreateVertices()
{
	int vertexNumber = 0;
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			// Old height: FMath::RandRange(0, 50)
			// Creates vertex positions in local space
			float perlinValue = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, vertexNumber + 0.01) * scale;

			if (perlinValue < 0) perlinValue = -perlinValue;
			// Create one per loop and connect in a later loop
			vertices.Add(FVector(i * 100, j * 100, perlinValue));

			// Add normals and tangents
			normals.Add(FVector(0, 0, 1));
			tangents.Add(FProcMeshTangent(0, 0, -1));
			// vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
			vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
			vertexNumber++;
			
			// Sets up UVs
			UV0.Add(FVector2D(0, 0));
		}
	}
}

// Joins the vertices as triangles
void AProcGenActor::JoinVertices()
{
	int vertexNumber = 0;
	// -1 on loops to avoid edges, which would cause out of bounds errors or really weird triangle connections
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			Triangles.Add(vertexNumber);				// Vertex point
			Triangles.Add(vertexNumber + 1);			// Vertex next to it
			Triangles.Add(vertexNumber + xSize + 1);	// Vertex diagonal from it
			Triangles.Add(vertexNumber);				// Vertex point
			Triangles.Add(vertexNumber + xSize + 1);	// Vertex diagonal from it
			Triangles.Add(vertexNumber + xSize);		// Vertex below it

			vertexNumber++; // Moves to next vertex
			/*if (j == ySize - 1)
			{
				vertexNumber++;	// Skips the edge vertex
			}*/
		}
	}
}

void AProcGenActor::CombineInformation()
{
	// Combines all the information and adds it to the mesh
	actorMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColours, tangents, true);
	
	// Enables collision
	actorMesh->ContainsPhysicsTriMeshData(true);

	// Look for a way to make unreal calculate normals
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, Triangles, UV0, normals, tangents);
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
	double value = Noise->perlinNoise.GetValue(1.25, 0.75, 0.50);
	UE_LOG(LogTemp, Warning, TEXT("Perlin hello world value: %f"), value);
}
