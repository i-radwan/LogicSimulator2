#include "XNOR2.h"
#include"..\ApplicationManager.h"
#include<fstream>

XNOR2::XNOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{
}


void XNOR2::Operate()
{
	//caclulate the output status as the XNORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == m_InputPins[1].getStatus()) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input XNOR gate
void XNOR2::Draw(Output* pOut)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete()) pOut->DrawXor_Xnor(m_CenterInfo, 2, true);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int XNOR2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int XNOR2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void XNOR2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void XNOR2::selectYourSelf(Output* pOut, color Color) {

}

void XNOR2::save(int id, ofstream & file)
{

	file << "XNOR2  " << id << "  "/* << this->getLabel() << "  " */<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void XNOR2::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}