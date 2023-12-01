// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Http.h"
#include "CoreMinimal.h"

#include "AssetRegistryModule.h"
#include "Engine/Texture.h"

#include "GameFramework/Actor.h"
#include "OGX_Server.generated.h"


UCLASS()
class OGX_STARS_API AOGX_Server : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOGX_Server();
	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
    void PyStartTask(int Num);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void HttpStartTask(int Num);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSuccessGenerate(const TArray<FColor>& XColors, const TArray<FVector2D>& XCenters, const FString& HDRIPath, const FString& HDRAPath);

	UFUNCTION(BlueprintImplementableEvent)
	void TestVector(const TArray<FColor>& Values);
	UFUNCTION(BlueprintImplementableEvent)
	void TestString(const FString& Strings);

	// UFUNCTION(BlueprintImplementableEvent)
	// void ShowYolov5Result(int Count, const TArray<float>& CenterX, const TArray<float>& CenterY);
	
	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void NormalizeCenterLocation(FVector2D Center, FVector& OutLocation);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void PickBlackHoleLocation(TArray<FVector2D> Centers, TArray<FVector2D>& RestCenters, FVector& BlackHoleCenters);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void GetColorsVariants(TArray<FColor> Colors, FColor& OutColor1, FColor& OutColor2, FColor& OutColor3, FColor& OutColor4, FColor& OutColor5);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void GetPlanetColorSet(TArray<FColor>& OutColors);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void GetPlanterRandomSettings(float& Inner, float& Outer, float& Frequency, float& Density, float& Seed);
	
	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void RandomPlanetTexture(UTexture*& OutTexture);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void SetStarMaterialInstanceColor(UMaterialInstanceDynamic* MaterialInstance);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	TArray<UTexture*> GetTexturesFromPath(FString Path);

	UFUNCTION(BlueprintCallable, Category = "OGX_Server")
	void SetTextures(TArray<UTexture*> DirTextures);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CenterRange = FVector2D(2000, 2000);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HeightRange = FVector2D(-200, 1000);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MidPoint = FVector2D(255, 255);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ColorOffset = 40;

	
	TArray<FColor> ServerColors;
	TArray<FVector2D> ServerCenters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture*> AssetTextures;

	TArray<FColor> StarColorPresets1 = {
		FColor(209, 46, 10),
		FColor(102, 61, 0),
		FColor(255, 41, 8),
		FColor(255, 26, 0),
		FColor(255, 26, 0)
	};
	TArray<FColor> StarColorPresets2 = {
		FColor(63, 135, 209),
		FColor(30, 4, 32),
		FColor(37, 209, 255),
		FColor(52, 107, 255),
		FColor(52, 187, 255)
	};


protected:
	FString TaskId;
	FTimerHandle TimerHandle;
	int StatusCode = 100;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
    void HttpCheckTask();
	void SetDelayTimer(int Seconds);

    void OnReceiveTaskId(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnPolling(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FColor CalculateColorOffset(FColor OriginalColor) const;
	// TArray<UTexture*> GetTexturesFromPath(FString Path);
	TArray<FColor> GetRandomStarColorSet();

	FString GeneratedPath;
	FString InpaintedPath;
	FString UpscalePath;
};
