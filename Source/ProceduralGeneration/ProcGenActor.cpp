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
	SetColours();
	CombineInformation();
	SetMaterial();
}

// Called when actor is already in a level that is opened
void AProcGenActor::PostLoad()
{
	Super::PostLoad();
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
			float perlinValue = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, (vertexNumber / 2) + 0.01) * scale;

			if (perlinValue < 0) perlinValue = -perlinValue;
			// Create one per loop and connect in a later loop
			vertices.Add(FVector(i * 100, j * 100, perlinValue));

			// Add normals and tangents
			normals.Add(FVector(0, 0, 1));
			tangents.Add(FProcMeshTangent(0, 0, -1));
			
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
	for (int i = 0; i < xSize - 2; i++)
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
			if (vertexNumber % ySize == ySize - 1)
			{
				vertexNumber++;	// Skips the edge vertex
			}
		}
	}
}

void AProcGenActor::SetColours()
{
	int vertexNumber = 0;
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			switch (BiomeType)
			{
			// Full white
			case colours::SNOW:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
				break;

			// Full green
			case colours::GRASS:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / (scale * 10)));
				break;

			// Aims for a Pale yellow colour to reflect sand
			case colours::DESERT:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / (scale * 2)));
				break;
			
			// Deep blue
			case colours::WATER:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / scale));
				break;

			// Dark red/brownish like an arid desert
			case colours::ARID:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 1.5), vertices[vertexNumber].Z / (scale * 4), vertices[vertexNumber].Z / (scale * 10)));
				break;

			// Just RED
			case colours::RED:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, 0.0f, 0.0f));
				break;

			// Defaults to SNOW colours
			default:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
				break;
			}
			vertexNumber++;
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
colours::Biome AProcGenActor::GetBiomeType() const
{
	return colours::Biome();
}

void AProcGenActor::SetBiomeType(colours::Biome biomeType)
{
	BiomeType = biomeType;
}

// Called when the game starts or when spawned
void AProcGenActor::BeginPlay()
{
	Super::BeginPlay();
	double value = Noise->perlinNoise.GetValue(1.25, 0.75, 0.50);
	UE_LOG(LogTemp, Warning, TEXT("Perlin hello world value: %f"), value);
}