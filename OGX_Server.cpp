// Fill out your copyright notice in the Description page of Project Settings.
#include "OGX_Server.h"
#include "AssetRegistryModule.h"
#include "Engine/Texture.h"
#include "Http.h"
#include "Core/Tests/Containers/TestUtils.h"


// Sets default values
AOGX_Server::AOGX_Server()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOGX_Server::PyStartTask(int Num)
{
	this->HttpStartTask(Num);
}

// Called when the game starts or when spawned
void AOGX_Server::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOGX_Server::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOGX_Server::HttpStartTask(int Num)
{
	UE_LOG(LogTemp, Log, TEXT("Initiate HTTP Request"));
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	FString URL = FString::Printf(TEXT("http://127.0.0.1:8000/myapp/start-task/?num=%d"), Num);
	Request->SetURL(URL);
	Request->SetVerb("GET"); // 或者 "POST"

	UE_LOG(LogTemp, Log, TEXT("Sent HTTP Request"));

	Request->OnProcessRequestComplete().BindUObject(this, &AOGX_Server::OnReceiveTaskId);
	Request->ProcessRequest();
}

void AOGX_Server::NormalizeCenterLocation(FVector2D Center, FVector& OutLocation)
{
	int AdjustX = (Center.X - MidPoint.X) / (MidPoint.X + 1) * CenterRange.X;
	int AdjustY = (Center.Y - MidPoint.Y) / (MidPoint.Y + 1) * CenterRange.Y;
	int RandomZ = FMath::RandRange(HeightRange.X, HeightRange.Y);
	UE_LOG(LogTemp, Warning, TEXT("X: %d, Y: %d, Z: %d"), AdjustX, AdjustY, RandomZ);
	OutLocation = FVector(AdjustX, AdjustY, RandomZ);
}

void AOGX_Server::PickBlackHoleLocation(TArray<FVector2D> Centers, TArray<FVector2D>& RestCenters,
	FVector& BlackHoleCenters)
{
	// Find the center which has the lowest Y value
	int LowestY = 512;
	int LowestIndex = 0;
	for (int i = 0; i < Centers.Num(); i++)
	{
		if (Centers[i].Y < LowestY)
		{
			LowestY = Centers[i].Y;
			LowestIndex = i;
		}
	}
	// Remove the lowest center from the array
	FVector2D LowestCenter = Centers[LowestIndex];
	Centers.RemoveAt(LowestIndex);
	RestCenters = Centers;
	// Normalize the location of the lowest center
	NormalizeCenterLocation(LowestCenter, BlackHoleCenters);
}

void AOGX_Server::GetColorsVariants(TArray<FColor> Colors, FColor& OutColor1, FColor& OutColor2, FColor& OutColor3,
	FColor& OutColor4, FColor& OutColor5)
{
	OutColor1 = CalculateColorOffset(Colors[0]);
	OutColor2 = CalculateColorOffset(Colors[1]);
	OutColor3 = CalculateColorOffset(Colors[2]);
	OutColor4 = CalculateColorOffset(Colors[3]);
	OutColor5 = CalculateColorOffset(Colors[4]);
}

void AOGX_Server::GetPlanetColorSet(TArray<FColor>& OutColors)
{
	const int RandomInt = FMath::RandRange(0, 10);
	// Branch 0-4, 5, 6, 7, 8, 9
	if (RandomInt >= 0 && RandomInt <= 4)
	{
		OutColors = {
			CalculateColorOffset(ServerColors[0]),
			CalculateColorOffset(ServerColors[1]),
			CalculateColorOffset(ServerColors[2]),
			CalculateColorOffset(ServerColors[3]),
			CalculateColorOffset(ServerColors[4])
		};
	}
	else if (RandomInt == 5)
	{
		OutColors = {
			FColor(15, 207, 252),
			FColor(238, 230, 233),
			FColor(250, 199, 7),
			FColor(229, 220, 5),
			FColor(5, 208, 254)
		};
	}
	else if (RandomInt == 6)
	{
		OutColors = {
			FColor(123, 122, 171),
			FColor(83, 120, 115),
			FColor(83, 94, 116),
			FColor(85, 113, 154),
			FColor(85, 97, 134)
		};

	}
	else if (RandomInt == 7)
	{
		OutColors = {
			FColor(101, 128, 130),
			FColor(114, 129, 134),
			FColor(129, 99, 127),
			FColor(93, 127, 167),
			FColor(84, 109, 130)
		};

	}
	else if (RandomInt == 8)
	{
		OutColors = {
			FColor(186, 219, 245),
			FColor(164, 200, 232),
			FColor(155, 222, 247),
			FColor(193, 233, 9),
			FColor(210, 221, 251)
		};

	}
	else if (RandomInt == 9)
	{
		OutColors = {
			FColor(172, 198, 245),
			FColor(156, 214, 26),
			FColor(173, 233, 3),
			FColor(170, 208, 13),
			FColor(211, 185, 26)
		};

	}
	else
	{
		OutColors = {
			FColor(136, 128, 135),
			FColor(106, 104, 129),
			FColor(83, 105, 169),
			FColor(105, 100, 146),
			FColor(121, 128, 139)
		};
	}
}

void AOGX_Server::GetPlanterRandomSettings(float& Inner, float& Outer, float& Frequency, float& Density, float& Seed)
{
	// Generate Random float range between 0.01 and 0.15
	Inner = FMath::RandRange(0.01f, 0.15f);
	// Generate Random float range between 0.05 and 0.4
	Outer = FMath::RandRange(0.05f, 0.4f);
	// Generate Random float range between 1 and 5
	Frequency = FMath::RandRange(1.0f, 5.0f);
	// Generate Random float range between -1 and 1
	Density = FMath::RandRange(-1.0f, 1.0f);
}


void AOGX_Server::RandomPlanetTexture(UTexture*& OutTexture)
{
	int TotalTextures = AssetTextures.Num();
	int RandomIndex = FMath::RandRange(0, TotalTextures - 1);
	OutTexture = AssetTextures[RandomIndex];
}

void AOGX_Server::SetStarMaterialInstanceColor(UMaterialInstanceDynamic* MaterialInstance)
{
	TArray<FColor> OutColors = GetRandomStarColorSet();
	MaterialInstance->SetVectorParameterValue(FName("ColorAtmo"), OutColors[0]);
	MaterialInstance->SetVectorParameterValue(FName("ColorFlame"), OutColors[1]);
	MaterialInstance->SetVectorParameterValue(FName("ColorFlare"), OutColors[2]);
	MaterialInstance->SetVectorParameterValue(FName("ColorHigh"), OutColors[3]);
	MaterialInstance->SetVectorParameterValue(FName("ColorLow"), OutColors[4]);
}



void AOGX_Server::HttpCheckTask()
{
	UE_LOG(LogTemp, Log, TEXT("Start Polling"));

	FString URL = FString::Format(TEXT("http://127.0.0.1:8000/myapp/check-task/{0}/"), { TaskId });

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &AOGX_Server::OnPolling);
	Request->ProcessRequest();

}

void AOGX_Server::SetDelayTimer(int Seconds)
{
	GetWorld()->GetTimerManager().SetTimer(
	   TimerHandle, 
	   this, 
	   &AOGX_Server::HttpCheckTask, 
	   Seconds, 
	   false
   );
}

void AOGX_Server::OnReceiveTaskId(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid())
	{
		// 将响应体转换为JSON
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			// 从JSON对象中获取task_id
			if (JsonObject->HasField("task_id"))
			{
				TaskId = JsonObject->GetStringField("task_id");
				StatusCode = 201;
				// 开始等待30秒后发送下一个请求的逻辑
				SetDelayTimer(30);
			}
		}
	}
}

void AOGX_Server::OnPolling(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// 将响应体转换为JSON
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			// 从JSON对象中获取task_id
			if (JsonObject->HasField("code"))
			{
				const FString Code = JsonObject->GetStringField("code");
				UE_LOG(LogTemp, Log, TEXT("Status CODE : %s"), *Code);
				if (Code == "202")
				{
					// 开始等待30秒后发送下一个请求的逻辑
					StatusCode = 202;
					SetDelayTimer(5);
				}
				else if (Code == "200")
				{
					FString LocalHDRIPath;
					FString LocalHDRAPath;
					StatusCode = 200;
					// 开始等待30秒后发送下一个请求的逻辑
					const TSharedPtr<FJsonObject> ResultObject = JsonObject->GetObjectField("result");
					if (ResultObject.IsValid())
					{
						GeneratedPath = ResultObject->GetStringField("generated_path");
						InpaintedPath = ResultObject->GetStringField("inpainted_path");
						UpscalePath = ResultObject->GetStringField("upscale_path");
						LocalHDRIPath = ResultObject->GetStringField("hdri_path");
						LocalHDRAPath = ResultObject->GetStringField("hdra_path");
						//Colors = ResultObject->GetArrayField("colors");
						TArray<TSharedPtr<FJsonValue>> ColorsJsonArray = ResultObject->GetArrayField("colors");
						for (const TSharedPtr<FJsonValue> ColorValue : ColorsJsonArray)
						{
							TArray<TSharedPtr<FJsonValue>> ColorComponents = ColorValue->AsArray();
							FColor Color;
							Color.R = ColorComponents[0]->AsNumber();
							Color.G = ColorComponents[1]->AsNumber();
							Color.B = ColorComponents[2]->AsNumber();
							ServerColors.Add(Color);
						}
						TArray<TSharedPtr<FJsonValue>> CentersJsonArray = ResultObject->GetArrayField("centers");
						for (const TSharedPtr<FJsonValue> CenterValue : CentersJsonArray)
						{
							TArray<TSharedPtr<FJsonValue>> CenterComponents = CenterValue->AsArray();
							FVector2D Center;
							Center.X = CenterComponents[0]->AsNumber();
							Center.Y = CenterComponents[1]->AsNumber();
							ServerCenters.Add(Center);
						}
					}
					OnSuccessGenerate(ServerColors, ServerCenters, LocalHDRIPath, LocalHDRAPath);
				}
				else
				{
					StatusCode = 500;
				}
			}
		}
	}
}

FColor AOGX_Server::CalculateColorOffset(FColor OriginalColor) const
{
	int32 NewR = FMath::Clamp(OriginalColor.R + FMath::RandRange(-ColorOffset, ColorOffset), 0, 255);
	int32 NewG = FMath::Clamp(OriginalColor.G + FMath::RandRange(-ColorOffset, ColorOffset), 0, 255);
	int32 NewB = FMath::Clamp(OriginalColor.B + FMath::RandRange(-ColorOffset, ColorOffset), 0, 255);

	// 返回新的颜色
	return FColor(NewR, NewG, NewB, OriginalColor.A);
}

TArray<UTexture*> AOGX_Server::GetTexturesFromPath(FString Path)
{
	TArray<UTexture*> Textures;
	// 获取所有的Texture资源
	TArray<FAssetData> AssetData;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.ClassPaths.Add(UTexture::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(*Path);
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	
	for (const FAssetData& Asset : AssetData)
	{
		UTexture* Texture = Cast<UTexture>(Asset.GetAsset());
		if (Texture)
		{
			Textures.Add(Texture);
		}
	}
	AssetTextures = Textures;
	return Textures;
}

void AOGX_Server::SetTextures(TArray<UTexture*> DirTextures)
{
	AssetTextures = DirTextures;
}

TArray<FColor> AOGX_Server::GetRandomStarColorSet()
{
	const int RandomInt = FMath::RandRange(0, 8);
	// Branch 0, 1-2, 3-5, 6-8
	if (RandomInt == 0)
	{
		return ServerColors;
	}
	if (RandomInt >= 1 && RandomInt <= 2)
	{
		return  {
			CalculateColorOffset(ServerColors[0]),
			CalculateColorOffset(ServerColors[1]),
			CalculateColorOffset(ServerColors[2]),
			CalculateColorOffset(ServerColors[3]),
			CalculateColorOffset(ServerColors[4])
		};
	}
	if (RandomInt >= 3 && RandomInt <= 5)
	{
		return StarColorPresets1;
	}
	return StarColorPresets2;
}
