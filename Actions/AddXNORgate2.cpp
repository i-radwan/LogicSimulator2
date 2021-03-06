#include "AddXNORgate2.h"
AddXNORgate2::AddXNORgate2(ApplicationManager *pApp) :Action(pApp)
{
}

AddXNORgate2::~AddXNORgate2(void)
{
}
//Check AddANDgate2 for the explanation of the next lines of code

bool AddXNORgate2::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintStatusBox(" 2-Input XNOR gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_XNOR_GATE_2, GInfo, smallImageBeforeAddingComponent, false, c)) {

		gateLabel =pIn->getStringBox();
		done = true;
	}

	
	
	return done;

}

void AddXNORgate2::Execute()
{
	//Get Center point of the Gate
	XNOR2*pA = new XNOR2({0,0,0,0}, FANOUT);

	image* smallImageBeforeAddingComponent = new image; if (ReadActionParameters(smallImageBeforeAddingComponent, pA)) {

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
