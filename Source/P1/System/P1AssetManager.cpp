// Fill out your copyright notice in the Description page of Project Settings.


#include "System/P1AssetManager.h"
#include "Data/P1AssetData.h"

UP1AssetManager::UP1AssetManager() : Super()
{

}

UP1AssetManager& UP1AssetManager::Get()
{
	if (UP1AssetManager* Singleton = Cast<UP1AssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Can't find P1AssetManager"));
	return *NewObject<UP1AssetManager>();
}

void UP1AssetManager::Initialize()
{
	Get().LoadPreloadAssets();
}

void UP1AssetManager::LoadSyncByPath(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		UObject* LoadedAsset = AssetPath.ResolveObject();
		if (LoadedAsset == nullptr)
		{
			if (UAssetManager::IsInitialized())
			{
				LoadedAsset = UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
			}
			else
			{
				LoadedAsset = AssetPath.TryLoad();
			}
		}

		if (LoadedAsset)
		{
			Get().AddLoadedAsset(AssetPath.GetAssetFName(), LoadedAsset);
		}
	}
}

void UP1AssetManager::LoadSyncByName(const FName& AssetName)
{
	UP1AssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	LoadSyncByPath(AssetPath);
}

void UP1AssetManager::LoadSyncByLabel(const FName& Label)
{
	if (UAssetManager::IsInitialized() == false)
	{
		return;
	}

	UP1AssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	TArray<FSoftObjectPath> AssetPaths;

	const FAssetSet& AssetSet = AssetData->GetAssetSetByLabel(Label);
	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FSoftObjectPath& AssetPath = AssetEntry.AssetPath;
		LoadSyncByPath(AssetPath);
		if (AssetPath.IsValid())
		{
			AssetPaths.Emplace(AssetPath);
		}
	}

	GetStreamableManager().RequestSyncLoad(AssetPaths);

	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FSoftObjectPath& AssetPath = AssetEntry.AssetPath;
		if (AssetPath.IsValid())
		{
			if (UObject* LoadedAsset = AssetPath.ResolveObject())
			{
				Get().AddLoadedAsset(AssetEntry.AssetName, LoadedAsset);
			}
		}
	}
}

void UP1AssetManager::ReleaseByPath(const FSoftObjectPath& AssetPath)
{
	FName AssetName = AssetPath.GetAssetFName();
	ReleaseByName(AssetName);
}

void UP1AssetManager::ReleaseByName(const FName& AssetName)
{
	UP1AssetManager& AssetManager = Get();
	if (AssetManager.NameToLoadedAsset.Contains(AssetName))
	{
		AssetManager.NameToLoadedAsset.Remove(AssetName);
	}
}

void UP1AssetManager::ReleaseByLabel(const FName& Label)
{
	UP1AssetManager& AssetManager = Get();
	UP1AssetData* LoadedAssetData = AssetManager.LoadedAssetData;
	const FAssetSet& AssetSet = LoadedAssetData->GetAssetSetByLabel(Label);

	for (const FAssetEntry& AssetEntry : AssetSet.AssetEntries)
	{
		const FName& AssetName = AssetEntry.AssetName;
		if (AssetManager.NameToLoadedAsset.Contains(AssetName))
		{
			AssetManager.NameToLoadedAsset.Remove(AssetName);
		}
	}
}

void UP1AssetManager::ReleaseAll()
{
	UP1AssetManager& AssetManager = Get();
	AssetManager.NameToLoadedAsset.Reset();
}

void UP1AssetManager::LoadPreloadAssets()
{
	if (LoadedAssetData)
		return;

	UP1AssetData* AssetData = nullptr;
	FPrimaryAssetType PrimaryAssetType(UP1AssetData::StaticClass()->GetFName());
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
	if (Handle.IsValid())
	{
		Handle->WaitUntilComplete(0.f, false);
		AssetData = Cast<UP1AssetData>(Handle->GetLoadedAsset());
	}
	if (AssetData)
	{
		LoadedAssetData = AssetData;
		LoadSyncByLabel("Preload");
	}
}

void UP1AssetManager::AddLoadedAsset(const FName& AssetName, const UObject* Asset)
{
	if (AssetName.IsValid() && Asset)
	{
		if (NameToLoadedAsset.Contains(AssetName) == false)
		{
			NameToLoadedAsset.Add(AssetName, Asset);
		}
	}
}


