#include "System/P1GameInstance.h"
#include "P1AssetManager.h"

UP1GameInstance::UP1GameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UP1GameInstance::Init()
{
	Super::Init();

	UP1AssetManager::Initialize();

}

void UP1GameInstance::Shutdown()
{
	Super::Shutdown();


}
