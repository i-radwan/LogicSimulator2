#include "AddNOTgate.h"
AddNOTgate::AddNOTgate(ApplicationManager *pApp) :Action(pApp)
{
}

AddNOTgate::~AddNOTgate(void)
{
}

bool AddNOTgate::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	bool done = false;

	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	pOut->PrintMsg(" NOT gate : Click to add the gate");

	if (pOut->SetDragImage(ADD_INV, GInfo, smallImageBeforeAddingComponent)){
		string s = "Please enter gate label: ";
		pOut->PrintMsg(s);
		gateLabel = pIn->GetSrting(pOut, s);
		done = true;
	}


	//Clear Status Bar
	pOut->ClearStatusBar();
	return done;

}

void AddNOTgate::Execute()
{
	//Get Center point of the Gate
	  image* smallImageBeforeAddingComponent = new image; if (ReadActionParameters(smallImageBeforeAddingComponent)){

	//Calculate the rectangle Corners
	int Len = UI.GATE_Width;
	int Wdth = UI.GATE_Height;

	//Gfx info to be used to construct the AND2 gate
	GraphicsInfo GInfotmp;

	GInfotmp.x1 = GInfo.x1 - Len / 2;
	GInfotmp.x2 = GInfo.x1 + Len / 2;
	GInfotmp.y1 = GInfo.y1 - Wdth / 2;
	GInfotmp.y2 = GInfo.y1 + Wdth / 2;
	NOT *pA = new NOT(GInfotmp, AND2_FANOUT);
	pManager->vec.push_back(GInfotmp);pManager->AddComponent(pA);pA->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);}
}

void AddNOTgate::Undo()
{}

void AddNOTgate::Redo()
{}