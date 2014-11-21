#pragma once

class FBspiiCommands : public TCommands<FBspiiCommands>
{
public:

	FBspiiCommands()
		: TCommands<FBspiiCommands>(TEXT("Bspii"), NSLOCTEXT("Contexts", "Bspii", "Bspii"), NAME_None, FEditorStyle::GetStyleSetName())
	{
		}

	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > MenuButton;

};