// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcGenActor.h"

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
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			// Creates vertex positions in local space
			vertices.Add(FVector(i * 100, j * 100, 0));
			vertices.Add(FVector(i * 100, 100 + (j * 100), 0));
			vertices.Add(FVector(100 + (i * 100), j * 100, 0));
			vertices.Add(FVector(100 + (i * 100), 100 + (j * 100), 50));
			
			// Adds the vertices to a triangle
			Triangles.Add(sectionNumber + 0);
			Triangles.Add(sectionNumber + 1);
			Triangles.Add(sectionNumber + 2);
			Triangles.Add(sectionNumber + 3);
			Triangles.Add(sectionNumber + 2);
			Triangles.Add(sectionNumber + 1);
			sectionNumber += 4;

			// Creates normals, tangents and sets up colours
			for (int k = 0; k < 4; k++)
			{
				normals.Add(FVector(0, 0, 1));
				tangents.Add(FProcMeshTangent(0, 0, -1));
				vertexColors.Add(FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f));
			}

			// Sets up UVs
			UV0.Add(FVector2D(0, 0));
			UV0.Add(FVector2D(1, 0));
			UV0.Add(FVector2D(0, 1));
			UV0.Add(FVector2D(1, 1));
		}
	}
	// Combines all the information and adds it to the mesh
	actorMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	// Enables collision
	actorMesh->ContainsPhysicsTriMeshData(true);
	SetMaterial();
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
	
}

