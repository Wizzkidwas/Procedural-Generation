// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcGenActor.h"

// Constructor
AProcGenActor::AProcGenActor()
{
	actorMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = actorMesh;
}


// Called when the actor is dropped into the world in the editor or at runtime
void AProcGenActor::PostActorCreated()
{
	Super::PostActorCreated();
	CreateTriangle();
}

// Called when actor is already in a level that is opened
void AProcGenActor::PostLoad()
{
	Super::PostLoad();
	CreateTriangle();
}

// Creates triangles and places them into the mesh
void AProcGenActor::CreateTriangle()
{	
	// Creates vertex positions in local space
	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(100, 0, 0));
	vertices.Add(FVector(100, 100, 50));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(100, 0, 0));

	// Adds the vertices to a triangle
	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);
	Triangles.Add(5);
	Triangles.Add(4);

	// Sets the normals and UVs
	TArray<FVector> normals;
	for (int i = 0; i < 6; i++)
	{
		normals.Add(FVector(1, 0, 0));
	}
	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));
	UV0.Add(FVector2D(10, 10));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));

	// Processes the tangents
	TArray<FProcMeshTangent> tangents;
	for (int i = 0; i < 6; i++)
	{
		tangents.Add(FProcMeshTangent(0, 0, -1));
	}

	// Adds colour
	TArray<FLinearColor> vertexColors;
	for (int i = 0; i < 6; i++)
	{
		vertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	// Combines all the information and adds it to the mesh
	actorMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	// Enables collision
	actorMesh->ContainsPhysicsTriMeshData(true);
}

// Called when the game starts or when spawned
void AProcGenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

