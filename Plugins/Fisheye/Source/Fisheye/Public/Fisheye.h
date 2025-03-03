// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameState.h"

class FToolBarBuilder;
class FMenuBuilder;
class UMaterialInstanceDynamic;

class FFisheyeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	void EnumLevel();


private:
	TSharedPtr<class FUICommandList> PluginCommands;

	UMaterial* FisheyeMaterial;
};
