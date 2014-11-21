// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "BspiiPrivatePCH.h"
#include "Bspii.h"
#include "Formats/MapParser.h"

DEFINE_LOG_CATEGORY(BspiiLog);

void FBspii::StartupModule()
{
	UE_LOG(BspiiLog, Log, TEXT("%s"), TEXT("Bspii loaded :D"));

	// Utils
	BspiiUtils = Utils();

	#pragma region Menus Setup
	FBspiiCommands::Register();

	BspiiCommands = MakeShareable(new FUICommandList);

	BspiiCommands->MapAction(
		FBspiiCommands::Get().MenuButton,
		FExecuteAction::CreateRaw(this, &FBspii::OnMenuButtonClicked),
		FCanExecuteAction());


	ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtension = ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, BspiiCommands, FToolBarExtensionDelegate::CreateRaw(this, &FBspii::AddToolbarExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	BspiiExtensionManager = LevelEditorModule.GetToolBarExtensibilityManager();
	#pragma endregion
}

void FBspii::OnMenuButtonClicked()
{
	UE_LOG(BspiiLog, Log, TEXT("%s"), TEXT("OnMenuButtonClicked"));
	BspiiUtils.SpawnTestCubeBrush(FVector(100, 500, 500));

	MapParser mp = MapParser(&BspiiUtils);
	//mp.Load(FString("D:\\NeoLight\\dp\\NeoLight\\maps\\ship.map"));
	//mp.Load(FString("D:\\E1M1.MAP"));
	mp.Load(FString("D:\\B_NAIL0.MAP"));
}

void FBspii::ShutdownModule()
{
	if (BspiiExtensionManager.IsValid())
	{
		FBspiiCommands::Unregister();

		ToolbarExtender->RemoveExtension(ToolbarExtension.ToSharedRef());

		BspiiExtensionManager->RemoveExtender(ToolbarExtender);
	}
	else
	{
		BspiiExtensionManager.Reset();
	}
}

void FBspii::AddToolbarExtension(FToolBarBuilder &builder)
{
#define LOCTEXT_NAMESPACE "LevelEditorToolBar"

	UE_LOG(BspiiLog, Log, TEXT("Starting Extension logic"));

	FSlateIcon IconBrush = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.ViewOptions", "LevelEditor.ViewOptions.Small");

	builder.AddToolBarButton(FBspiiCommands::Get().MenuButton, NAME_None, LOCTEXT("MyButton_Override", "Bspii"), LOCTEXT("BspiiMenu_ToolTipOverride", "Opens a menu to import Q3 .maps"), IconBrush, NAME_None);

#undef LOCTEXT_NAMESPACE
}



