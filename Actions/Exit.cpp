#include "Exit.h"





Exit::Exit(ApplicationManager * pApp):Action(pApp)
{
	choice = -1;
}

Exit::~Exit()
{
}

bool Exit::ReadActionParameters(image *)
{

	choice = pManager->GetOutput()->printPopUpMessage("", 'Q');
	return true;
}

void Exit::Execute()
{
	// Exit action here
	if (ReadActionParameters()) {
		if (choice == IDNO) {
			pManager->setExitChoice(1);
			Clear clearAction(pManager);
			clearAction.setLoading(true);
			clearAction.Execute();
		}
		else if (choice == IDYES)
		{
			Action *pAct = new Save(pManager);
			pAct->Execute();
			if (((Save*)pAct)->isSuccessful()) {
				pManager->setExitChoice(1);
				Clear clearAction(pManager);
				clearAction.setLoading(true);
				clearAction.Execute();
			}
			else
				pManager->setExitChoice(0);
		}
		else
			pManager->setExitChoice(0);

	}
}

void Exit::Undo()
{
	//Nothing to do here
}

void Exit::Redo()
{
	//Nothing to do here
}
