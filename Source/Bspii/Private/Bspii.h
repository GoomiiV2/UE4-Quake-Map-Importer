#pragma once

#include "ModuleManager.h"
#include "LevelEditorActions.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "FBspiiCommands.h"
#include "Internationalization.h"
#include "Slate.h"
#include "Runtime/Slate/Public/Framework/MultiBox/MultiBoxExtender.h"
#include "Editor/UnrealEd/Public/BSPOps.h"
#include "UnrealEd.h"
#include "LevelEditor.h"
#include "Runtime/Engine/Classes/Engine/Brush.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Utils.h"

DECLARE_LOG_CATEGORY_EXTERN(BspiiLog, Log, All);

class FBspii : public IBspii
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	Utils BspiiUtils;

public:
	void OnMenuButtonClicked();

private:
	void AddToolbarExtension(FToolBarBuilder &);

	TSharedPtr<FUICommandList> BspiiCommands;
	TSharedPtr<FExtensibilityManager> BspiiExtensionManager;
	TSharedPtr< const FExtensionBase > ToolbarExtension;
	TSharedPtr<FExtender> ToolbarExtender;
};

IMPLEMENT_MODULE(FBspii, Bspii)