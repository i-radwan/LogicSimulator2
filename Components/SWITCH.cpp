#include "SWITCH.h"
#include"..\ApplicationManager.h"
#include<fstream>

SWITCH::SWITCH(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo, r_FanOut)
{
	m_OutputPin.setComponent(this);
	
}


void SWITCH::Operate()
{
}


// Function Draw
// Draws SWITCH
void SWITCH::Draw(Output* pOut)
{
	//Call output class and pass SWITCH drawing info to it.
	if (!getDelete())pOut->DrawSwtich(m_CenterInfo, (getOutputPin()->getStatus() == HIGH) ? true : false);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int SWITCH::GetOutPinStatus()
{
	return getOutputPin()->getStatus();
}


//returns status of Inputpin #n
int SWITCH::GetInputPinStatus(int n)
{
	return-1; //SWITCH
}

//Set status of an input pin to HIGH or LOW
void SWITCH::setOutputPinStatus(int n, STATUS s)
{
	getOutputPin()->setStatus(s);
}
void SWITCH::setInputPinStatus(int n, STATUS s)
{
	return;
}
SWITCH::~SWITCH(){

}
void SWITCH::selectYourSelf(Output* pOut, color Color) {

}

void SWITCH::save(int id, ofstream & file)
{

	file << "SWITCH  " << id << "  "/* << this->getLabel() << "  " */<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void SWITCH::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}