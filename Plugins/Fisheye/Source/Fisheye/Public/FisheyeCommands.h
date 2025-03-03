// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "FisheyeStyle.h"

class FFisheyeCommands : public TCommands<FFisheyeCommands>
{
public:

	FFisheyeCommands()
		: TCommands<FFisheyeCommands>(TEXT("Fisheye"), NSLOCTEXT("Contexts", "Fisheye", "Fisheye Plugin"), NAME_None, FFisheyeStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
