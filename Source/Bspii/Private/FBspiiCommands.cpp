#include "BspiiPrivatePCH.h"

#include "FBspiiCommands.h"
 
PRAGMA_DISABLE_OPTIMIZATION
void FBspiiCommands::RegisterCommands()
{
	UI_COMMAND(MenuButton, "Bspii Menu", "Opn the import menu", EUserInterfaceActionType::Button, FInputGesture());
}
PRAGMA_ENABLE_OPTIMIZATION
