#pragma once
#include <wx\wx.h>
#include <wx\spinctrl.h>
#include <wx\statline.h>
#include <cmath>
#include "weld.h"
#include "database.h"

class widget : public wxPanel {
public:
	widget(wxFrame *parent); //принимаем указатель на класс (myFrame)
	~widget();

private:
	weld *myWeld;

	enum widgetID {
		ID_ObjectBox,
		ID_WeldTypeBox,
		ID_MethodBox,
		ID_ThicknessSpin,
		ID_DiameterSpin,
		ID_LengthSpin,
		ID_GenButtonEvt,
		ID_DefectWindow,
		ID_SumWindow,
		ID_HeightWindow,
		ID_WidthWindow
	};

	wxArrayString chObject;
	wxRadioBox *object;
	wxArrayString chWeldType;
	wxRadioBox *weldType;
	wxArrayString chMethod;
	wxRadioBox *method;
	wxStaticText *thickText;
	wxSpinCtrl *thickSpin;
	wxStaticText *diameter;
	wxSpinCtrl *diamSpin;
	wxStaticText *length;
	wxSpinCtrl *lengthSpin;
	wxStaticText *length6;
	wxButton *buttonGenerate;
	wxTextCtrl *defectWindow;
	wxStaticText *sumLength;
	wxTextCtrl *sumWindow;
	wxStaticText *height;
	wxTextCtrl *heightWindow;
	wxStaticText *width;
	wxTextCtrl *widthWindow;
	wxStaticText *dispacement;
	wxTextCtrl *displacementWindow;
	wxStaticLine *line1;
	wxStaticText *params;

	void ObjectEvent(wxCommandEvent &event);
	void WeldTypeEvent(wxCommandEvent &event);
	void MethodEvent(wxCommandEvent &event);
	void ThicknessSpinEvent(wxCommandEvent &event);
	void DiameterSpinEvent(wxCommandEvent &event);
	void LengthSpinEvent(wxCommandEvent &event);
	void GenButtonEvent(wxCommandEvent &event);
};

