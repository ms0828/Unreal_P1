// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Data/P1AssetData.h"
#include "P1AssetManager.generated.h"

class UP1AssetData;

UCLASS()
class P1_API UP1AssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UP1AssetManager();
	
	static UP1AssetManager& Get();

public:
	static void Initialize();

	template<typename AssetType>
	static AssetType* GetAssetByName(const FName& AssetName);

	static void LoadSyncByPath(const FSoftObjectPath& AssetPath);
	static void LoadSyncByName(const FName& AssetName);
	static void LoadSyncByLabel(const FName& Label);

	static void ReleaseByPath(const FSoftObjectPath& AssetPath);
	static void ReleaseByName(const FName& AssetName);
	static void ReleaseByLabel(const FName& Label);
	static void ReleaseAll();

private:
	void LoadPreloadAssets();
	void AddLoadedAsset(const FName& AssetName, const UObject* Asset);

private:
	UPROPERTY()
	TObjectPtr<UP1AssetData> LoadedAssetData;


	//cashing
	UPROPERTY()
	TMap<FName, TObjectPtr<const UObject>> NameToLoadedAsset;
};

template<typename AssetType>
AssetType* UP1AssetManager::GetAssetByName(const FName& AssetName)
{
	UP1AssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	if (AssetPath.IsValid())
	{
		LoadedAsset = Cast<AssetType>(AssetPath.ResolveObject());
		if (LoadedAsset == nullptr)
		{
			LoadedAsset = Cast<AssetType>(AssetPath.TryLoad());
		}
	}
	return LoadedAsset;
}