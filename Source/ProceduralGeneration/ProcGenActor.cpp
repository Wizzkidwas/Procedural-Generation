// Fill out your copyright notice in the Description page of Project Settings.

#include <libnoise.h>
#include "ProcGenActor.h"

// Constructor
AProcGenActor::AProcGenActor()
{
	// Allows the mesh to be accessible in blueprints
	actorMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = actorMesh;

	// Creates a material object to set for the landscape
	meshMaterial = CreateDefaultSubobject<UMaterial>(TEXT("MeshMaterial"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("'/Game/Materials/BlankMaterial'"));
	meshMaterial = Material.Object;
	
	// Noise object accessible in blueprints, allows editing of values
	Noise = CreateDefaultSubobject< UPerlinNoise_ActorComponent>(TEXT("Noise"));
}

// Called when the actor is dropped into the world in the editor or at runtime
void AProcGenActor::PostActorCreated()
{
	Super::PostActorCreated();

	// Calls all the functions required to create the landscape
	// Collects paramters for the noise
	Noise->SetupOptions(Noise->GetFrequency(), Noise->GetLacunarity(), Noise->GetNoiseQuality(), Noise->GetOctaveCount(), Noise->GetPersistence(), Noise->GetSeed());
	
	// Sets up landscape vertices
	CreateVertices();
	
	// Joins vertices into a solid landscape
	JoinVertices();
	
	// Colours vertices based on biome selection
	SetColours();
	
	// Calls the create mesh function to combine all vertex data into a single mesh
	CombineInformation();
	
	// Sets the "blank material" so colours can be seen
	SetMaterial();
}

// Called when actor is already in a level that is opened
// This is left unmodified so the landscape does not change after it has been placed
void AProcGenActor::PostLoad()
{
	Super::PostLoad();
}

// Sets up vertices
void AProcGenActor::CreateVertices()
{
	int vertexNumber = 0;	// Keeps track of which vertex is being processed
	for (int i = 0; i < xSize; i++)	// Nested loop going column by column
	{
		for (int j = 0; j < ySize; j++)
		{
			// Creates vertex positions in local space
			float perlinValue = Noise->perlinNoise.GetValue(i * 0.001, j * 0.001, (vertexNumber / 2) + 0.01) * scale;

			// Prevents vertices being places below the "0 line"
			if (perlinValue < 0) perlinValue = -perlinValue;
			
			// Creates one vertex per loop which will be connected in a later loop
			vertices.Add(FVector(i * 100, j * 100, perlinValue));

			// Add normals and tangents
			normals.Add(FVector(0, 0, 1));
			tangents.Add(FProcMeshTangent(0, 0, -1));
			
			UV0.Add(FVector2D(0, 0)); // Sets up UVs

			vertexNumber++;	// Increments to next vertex
		}
	}
}

// Joins the vertices as triangles
void AProcGenActor::JoinVertices()
{
	int vertexNumber = 0;	// Keeps track of which vertex is being processed

	// -2 on outer loop to avoid processing the last row multiple times
	// If not done, this causes a strange looking "wall" at one end of the landscape
	for (int i = 0; i < xSize - 2; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			// Triangles are added as a Triangle List
			// Creates a quad from two triangles

			Triangles.Add(vertexNumber);				// Current Vertex
			Triangles.Add(vertexNumber + 1);			// Vertex next to it
			Triangles.Add(vertexNumber + xSize + 1);	// Vertex diagonal from it
			Triangles.Add(vertexNumber);				// Current Vertex 
			Triangles.Add(vertexNumber + xSize + 1);	// Vertex diagonal from it
			Triangles.Add(vertexNumber + xSize);		// Vertex on the row it

			vertexNumber++; // Increments to next vertex

			if (vertexNumber % ySize == ySize - 1)	// Uses a modulo to work for any ySize
			{
				vertexNumber++;	// Skips the edge vertex
				// If not done, edge vertices would connect to vertices on the opposite side
			}
		}
	}
}

// Considers chosen biome and sets the colours of the vertices based on it
void AProcGenActor::SetColours()
{
	int vertexNumber = 0;	// Keeps track of which vertex is being processed
	for (int i = 0; i < xSize; i++)
	{	// Goes through every vertex
		for (int j = 0; j < ySize; j++)
		{
			switch (BiomeType)	// Based on chosen Biome type
			{
			// Full white
			// Each colour weighted equally
			case colours::SNOW:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
				break;

			// Full green
			// R and B value weighted much less
			case colours::GRASS:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / (scale * 10)));
				break;

			// Aims for a Pale yellow colour to reflect sand
			// B colour weighted less favourably
			case colours::DESERT:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / (scale * 2)));
				break;
			
			// Deep blue
			// R and G value weighted much less
			case colours::WATER:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / (scale * 10), vertices[vertexNumber].Z / scale));
				break;

			// Dark red/brownish like an arid desert
			// R value weighted strongest, then G, then B weighted the least
			case colours::ARID:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / (scale * 1.5), vertices[vertexNumber].Z / (scale * 4), vertices[vertexNumber].Z / (scale * 10)));
				break;

			// Just RED
			// G and B values not weighted at all
			case colours::RED:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, 0.0f, 0.0f));
				break;

			// Defaults to SNOW colours
			default:
				vertexColours.Add(FLinearColor(vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale, vertices[vertexNumber].Z / scale));
				break;
			}

			vertexNumber++; // Increments to next vertex
		}
	}
}

// Creates mesh, enables collisions and calculates tangents
void AProcGenActor::CombineInformation()
{
	// Combines all the information and adds it to the mesh as "Section 0"
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
		auto MaterialInstance = UMaterialInstanceDynamic::Create(meshMaterial, this);	// Creates a material instance for the object
		actorMesh->SetMaterial(0, MaterialInstance);									// Sets the material for "Section 0", which covers the entire mesh
	}
}

// Getter for the Biome setting, used by the blueprint
colours::Biome AProcGenActor::GetBiomeType() const
{
	return colours::Biome();
}

// Setter for the Biome setting, used by the blueprint
void AProcGenActor::SetBiomeType(colours::Biome biomeType)
{
	BiomeType = biomeType;
}

// Called when the game starts or when spawned
// Used as a test to see if the noise library was implemented correctly
void AProcGenActor::BeginPlay()
{
	Super::BeginPlay();
	double value = Noise->perlinNoise.GetValue(1.25, 0.75, 0.50);
	UE_LOG(LogTemp, Warning, TEXT("Perlin hello world value: %f"), value);
}