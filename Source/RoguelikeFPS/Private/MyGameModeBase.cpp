#include "MyGameModeBase.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();

}
