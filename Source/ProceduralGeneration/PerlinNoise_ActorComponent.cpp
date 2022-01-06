// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoise_ActorComponent.h"

// Sets default values for this component's properties
UPerlinNoise_ActorComponent::UPerlinNoise_ActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UpdateOptions();
	
	// ...
}


// Called when the game starts
void UPerlinNoise_ActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
}


// Called every frame
void UPerlinNoise_ActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Getters
float UPerlinNoise_ActorComponent::GetFrequency() const
{
	return perlinNoise.GetFrequency();
}

float UPerlinNoise_ActorComponent::GetLacunarity() const
{
	return perlinNoise.GetLacunarity();
}

qualities::PerlinNoiseQuality UPerlinNoise_ActorComponent::GetNoiseQuality() const
{
	return convertBPNoiseQuality(perlinNoise.GetNoiseQuality());
}

int UPerlinNoise_ActorComponent::GetOctaveCount() const
{
	return perlinNoise.GetOctaveCount();
}

float UPerlinNoise_ActorComponent::GetPersistence() const
{
	return perlinNoise.GetPersistence();
}

int UPerlinNoise_ActorComponent::GetSeed() const
{
	return perlinNoise.GetSeed();
}

int UPerlinNoise_ActorComponent::GetSourceModuleCount() const
{
	return perlinNoise.GetSourceModuleCount();
}

float UPerlinNoise_ActorComponent::GetValue(float x, float y, float z) const
{
	float value = perlinNoise.GetValue(x, y, z);
	// UE_LOG(LogTemp, Warning, TEXT("Perlin value for: %f,%f,%f: %f"), x, y, z, value);
	return value;
}

// Setters
void UPerlinNoise_ActorComponent::SetFrequency(float frequency)
{
	return perlinNoise.SetFrequency(frequency);
}

void UPerlinNoise_ActorComponent::SetLacunarity(float lacunarity)
{
	return perlinNoise.SetLacunarity(lacunarity);
}

void UPerlinNoise_ActorComponent::SetNoiseQuality(qualities::PerlinNoiseQuality noiseQuality)
{
	return perlinNoise.SetNoiseQuality(convertBPNoiseQuality(noiseQuality));
}

void UPerlinNoise_ActorComponent::SetOctaveCount(int octaveCount)
{
	perlinNoise.SetOctaveCount(octaveCount);
}

void UPerlinNoise_ActorComponent::SetPersistence(float persistence)
{
	perlinNoise.SetPersistence(persistence);
}

void UPerlinNoise_ActorComponent::SetSeed(int seed)
{
	perlinNoise.SetSeed(seed);
}

void UPerlinNoise_ActorComponent::SetRandomSeed()
{
	SetSeed(FMath::RandRange(0, 1000000));
}

// Mass setter
void UPerlinNoise_ActorComponent::SetupOptions(float frequency, float lacunarity, qualities::PerlinNoiseQuality noiseQuality, int octaveCount, float persistence, int seed)
{
	SetFrequency(frequency);
	SetLacunarity(lacunarity);
	SetNoiseQuality(noiseQuality);
	SetOctaveCount(octaveCount);
	SetPersistence(persistence);
	SetSeed(seed);
}

// Quality
noise::NoiseQuality UPerlinNoise_ActorComponent::convertBPNoiseQuality(qualities::PerlinNoiseQuality quality) const
{
	switch (quality)
	{
	case qualities::QUALITY_FAST:
		return noise::QUALITY_FAST;
		break;
	case qualities::QUALITY_STD:
		return noise::QUALITY_STD;
		break;
	case qualities::QUALITY_BEST:
		return noise::QUALITY_BEST;
		break;
	default:
		return noise::QUALITY_STD;
	}
}

qualities::PerlinNoiseQuality UPerlinNoise_ActorComponent::convertBPNoiseQuality(noise::NoiseQuality quality) const
{
	switch (quality)
	{
	case noise::QUALITY_FAST:
		return qualities::QUALITY_FAST;
		break;
	case noise::QUALITY_STD:
		return qualities::QUALITY_STD;
		break;
	case noise::QUALITY_BEST:
		return qualities::QUALITY_BEST;
		break;
	default:
		return qualities::QUALITY_STD;
	}
}

void UPerlinNoise_ActorComponent::UpdateOptions()
{
	SetupOptions(Frequency, Lacunarity, NoiseQuality, OctaveCount, Persistence, Seed);
}