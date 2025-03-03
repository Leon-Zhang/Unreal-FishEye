// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fisheye.h"
#include "FisheyeStyle.h"
#include "FisheyeCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "Engine/WorldComposition.h"
#include <Editor/WorldBrowser/Private/LevelModel.h>
#include <Editor/WorldBrowser/Public/WorldBrowserModule.h>
#include <Editor/WorldBrowser/Private/LevelCollectionModel.h>
#include <Engine/PostProcessVolume.h>

//#include <Engine\Classes\Materials\MaterialInstanceDynamic.h>

static const FName FisheyeTabName("Fisheye");

#define LOCTEXT_NAMESPACE "FFisheyeModule"

void FFisheyeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FFisheyeStyle::Initialize();
	FFisheyeStyle::ReloadTextures();

	FFisheyeCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FFisheyeCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FFisheyeModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FFisheyeModule::RegisterMenus));


	FisheyeMaterial=LoadObject<UMaterial>(nullptr,TEXT("/Fisheye/Material/Fisheye"));
	//TEXT("/StarterContent/Materials/M_Ceramic_Tile_Checker")
	//UMaterial* FisheyeMaterial2=LoadObject<UMaterial>(nullptr,TEXT("/Game/Fisheye"));
	//UE_LOG(LogTemp,Display,TEXT("AAreaLightingController: FindPostProcessVolume(): volume found: {0}!"),*_postProcessVolume->GetActorLabel());
}

void FFisheyeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FFisheyeStyle::Shutdown();

	FFisheyeCommands::Unregister();
}

UWorld* GetFirstWorld()
{
	if(GEngine){
		// Loop through all world contexts and find the first valid UWorld
		for(const FWorldContext& WorldContext:GEngine->GetWorldContexts()){
			if(WorldContext.World()){
				return WorldContext.World();
			}
		}
	}
	return nullptr;
}

void ToggleActorVisibility(AActor& Actor)
{
	// Get the current visibility state
	bool bIsHidden=Actor.IsHidden();

	// Toggle the visibility
	Actor.SetActorHiddenInGame(!bIsHidden);
	// Optionally, if you want to toggle collision as well:
	Actor.SetActorEnableCollision(bIsHidden);
}



void FFisheyeModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*FText DialogText=FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FFisheyeModule::PluginButtonClicked()")),
							FText::FromString(TEXT("Fisheye.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok,DialogText);*/

	// find post process volume
	APostProcessVolume* _postProcessVolume=NULL;
	for(TActorIterator<APostProcessVolume> ActorItr(GetFirstWorld()); ActorItr; ++ActorItr){
		_postProcessVolume=Cast<APostProcessVolume>(*ActorItr);
		if(_postProcessVolume){
			bool bHidden;
			if(FisheyeMaterial && _postProcessVolume->Settings.WeightedBlendables.Array.IsEmpty()){

				UMaterialInstanceDynamic* FisheyeMaterialInst=UMaterialInstanceDynamic::Create(FisheyeMaterial,nullptr);
				if(FisheyeMaterialInst){
					FisheyeMaterialInst->SetScalarParameterValue(TEXT("Radius"),1.2365f);
					FisheyeMaterialInst->SetScalarParameterValue(TEXT("Density"),1.1431f);
					FisheyeMaterialInst->SetScalarParameterValue(TEXT("MulB"),1.0122f);
				}
				_postProcessVolume->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f,FisheyeMaterialInst));
				//	else
				//		_postProcessVolume->Settings.WeightedBlendables.Array[0]=FWeightedBlendable(1.0f,FisheyeMaterialInst);
				bHidden=false;
			} else{
				bHidden=!_postProcessVolume->IsTemporarilyHiddenInEditor();
			}
			
			_postProcessVolume->SetIsTemporarilyHiddenInEditor(bHidden);
			break;
		}
	}
}

void FFisheyeModule::EnumLevel()
{
	int count=FModuleManager::Get().GetModuleCount();
	FWorldBrowserModule* WorldBrowserModule=FModuleManager::Get().GetModulePtr<FWorldBrowserModule>("WorldBrowser");
	if(!this->IsGameModule()&&WorldBrowserModule){

		// Getting model which contains list of all SubLevels
		//FWorldBrowserModule& WorldBrowserModule=FModuleManager::GetModuleChecked<FWorldBrowserModule>("WorldBrowser");
		TSharedPtr<FLevelCollectionModel> SharedWorldModel=WorldBrowserModule->SharedWorldModel(GEngine->GetWorld());

		// Now iterating by all SubLevels
		for(TSharedPtr<FLevelModel> Child:SharedWorldModel->GetAllLevels()){
			if(Child.Get()->GetDisplayName()=="Persistent Level"){
				// Will return the name of the level - HierarchyTest from the example above
				FString LevelName=GEngine->GetWorld()->GetMapName();
			} else{
				// Will return HierarchyTest_Sublevel1 from the example above
				FString SubLevelName=Child.Get()->GetDisplayName();
				// Will return SubFolder1 from the example above
				FName SubLevelPath=Child.Get()->GetFolderPath();
			}
			UE_LOG(LogTemp,Display,TEXT("Child Level: %s"),*Child.Get()->GetDisplayName());
		}


		if(UWorld* World=GEditor->GetCurrentPlayWorld()){
			auto StreamingLevels=World->GetStreamingLevels();
			for(ULevelStreaming* StreamingLevel:StreamingLevels){
				ULevel* level=StreamingLevel->GetLoadedLevel();
				if(!level) continue;
				for(AActor* Actor:level->Actors){
					// Actor action
				}
			}

			TActorIterator<AActor> Iterator(World,AActor::StaticClass());
			for(; Iterator; ++Iterator){
				AActor* Actor=Cast<AActor>(*Iterator);

				FString name=Actor->GetName();
				if(name=="PostProcessVolume"){
					USceneComponent* node=Cast<USceneComponent>(Actor);
					node->ToggleVisibility(false);
				}

				TArray<UStaticMeshComponent*> SMeshComponents;
				Actor->GetComponents<UStaticMeshComponent>(SMeshComponents);
				if(!SMeshComponents.IsEmpty()){

					UE_LOG(LogTemp,Display,TEXT("Actor ID: %s"),*Actor->GetName());
					//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, Actor->GetFName().ToString());  //Print name of the Actor
					//another code
				}
			}
		}
	}
}

void FFisheyeModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FFisheyeCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FFisheyeCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFisheyeModule, Fisheye)