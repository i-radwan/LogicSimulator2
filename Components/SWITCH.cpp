#include "SWITCH.h"

SWITCH::SWITCH(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;

	outkey = new OutputPin(r_FanOut);
}


void SWITCH::Operate()
{
}


// Function Draw
// Draws SWITCH
void SWITCH::Draw(Output* pOut)
{
	//Call output class and pass SWITCH drawing info to it.
	pOut->DrawSwtich(m_GfxInfo, (outkey->getStatus() == HIGH) ? true : false);
}

//returns status of outputpin
int SWITCH::GetOutPinStatus()
{
	return outkey->getStatus();
}


//returns status of Inputpin #n
int SWITCH::GetInputPinStatus(int n)
{
	return-1; //SWITCH
}

//Set status of an input pin to HIGH or LOW
void SWITCH::setOutputPinStatus(int n, STATUS s)
{
	outkey->setStatus(s);
}
void SWITCH::setInputPinStatus(int n, STATUS s)
{
	return;
}
SWITCH::~SWITCH(){

}