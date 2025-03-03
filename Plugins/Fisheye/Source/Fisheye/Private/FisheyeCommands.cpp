// Copyright Epic Games, Inc. All Rights Reserved.

#include "FisheyeCommands.h"

#define LOCTEXT_NAMESPACE "FFisheyeModule"

void FFisheyeCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Fisheye", "Execute Fisheye action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
