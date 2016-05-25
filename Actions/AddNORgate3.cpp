#include "AddNORgate3.h"
AddNORgate3::AddNORgate3(ApplicationManager *pApp) :Action(pApp)
{
}

AddNORgate3::~AddNORgate3(void)
{
}
//Check AddANDgate2 for the explanation of the next lines of code

bool AddNORgate3::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox(" 3-Input NOR gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_NOR_GATE_3, GInfo, smallImageBeforeAddingComponent, false, c)) {

		gateLabel = pIn->getStringBox();
		done = true;
	}


	
	
	return done;

}

void AddNORgate3::Execute()
{
	//Get Center point of the Gate
	image* smallImageBeforeAddingComponent = new image;
	NOR3 *pA = new NOR3({ 0,0,0,0 }, FANOUT);

	if (ReadActionParameters(smallImageBeforeAddingComponent, pA)) {

		//Calculate the rectangle Corners
		int Len = UI.GATE_Width;
		int Wdth = UI.GATE_Height;
		GraphicsInfo GInfotmp;
		GInfotmp.x1 = GInfo.x1 - Len / 2;
		GInfotmp.x2 = GInfo.x1 + Len / 2;
		GInfotmp.y1 = GInfo.y1 - Wdth / 2;
		GInfotmp.y2 = GInfo.y1 + Wdth / 2;
		pA->setNewCenterLocation(GInfo);
		pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		Utils::allComponentsCorners.push_back(GInfotmp);
		pManager->AddComponent(pA);
		pA->setLabel(gateLabel);
		Utils::undoActions.push(this);
		Action::pA = pA;
	}
	else {
		delete pA;
		pA = NULL;
	}

}