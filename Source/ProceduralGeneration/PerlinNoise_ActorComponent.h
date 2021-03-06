// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <libnoise.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerlinNoise_ActorComponent.generated.h"


// UE4's version of an enum that corresponds to perlin noise qualities.
UENUM(BluePrintType)
namespace qualities
{
	enum PerlinNoiseQuality
	{
		// Internal name | Sets name to be used for the blueprint
		QUALITY_FAST	UMETA(DisplayName = "Fast"),
		QUALITY_STD		UMETA(DisplayName = "Standard"),
		QUALITY_BEST	UMETA(DisplayName = "Best")
	};
}

// Allows the class to be modified via blueprints
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROCEDURALGENERATION_API UPerlinNoise_ActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerlinNoise_ActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called when created
	virtual void OnComponentCreated() override;

public:
	// The main function to call to get noise output
	UFUNCTION(BluePrintCallable, meta = (Keywords = "noise perlin"), Category = "Perlin")
		float GetValue(float x, float y, float z) const;

	// Simple setup function that can set all the options at once
	UFUNCTION(BluePrintCallable, Category = "Perlin")				// Below is the enum
		void SetupOptions(float frequency, float lacunarity, qualities::PerlinNoiseQuality noiseQuality, int octaveCount, float persistence, int seed);

	// Setters/getters for all the individual noise properties
	// "BluePrintCallable" allows functions to be used from the blueprints
	// All functions are in the "Perlin" category for blueprints
	UFUNCTION(BluePrintCallable, Category = "Perlin")
		float GetFrequency() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		float GetLacunarity() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		qualities::PerlinNoiseQuality GetNoiseQuality() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		int GetOctaveCount() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		float GetPersistence() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		int GetSeed() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		int GetSourceModuleCount() const;

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetFrequency(float frequency);

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetLacunarity(float lacunarity);

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetNoiseQuality(qualities::PerlinNoiseQuality noiseQuality);

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetOctaveCount(int octaveCount);

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetPersistence(float persistence);

	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetSeed(int seed);

	// Function below is a convenient function to set a random seed
	UFUNCTION(BluePrintCallable, Category = "Perlin")
		void SetRandomSeed();

public:
	noise::module::Perlin perlinNoise;

private:
	noise::NoiseQuality convertBPNoiseQuality(qualities::PerlinNoiseQuality quality) const;
	qualities::PerlinNoiseQuality convertBPNoiseQuality(noise::NoiseQuality quality) const;
	void UpdateOptions();

	// These are private because they shouldn't be modified at runtime, but can be edited in the editor before running the game
	// Default values
	UPROPERTY(EditAnywhere, Category = "Perlin")
		float Frequency = 1.0;
	UPROPERTY(EditAnywhere, Category = "Perlin")
		float Lacunarity = 2.0;
	UPROPERTY(EditAnywhere, Category = "Perlin")
		TEnumAsByte<qualities::PerlinNoiseQuality> NoiseQuality = qualities::QUALITY_STD;
	UPROPERTY(EditAnywhere, Category = "Perlin")
		int OctaveCount = 6;
	UPROPERTY(EditAnywhere, Category = "Perlin")
		float Persistence = 0.5;
	UPROPERTY(EditAnywhere, Category = "Perlin")
		int Seed = 0;
};