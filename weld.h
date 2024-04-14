#pragma once
#include <random>
#include <chrono>
#include <wx/string.h>
#include <wx/vector.h>
#include "defect.h"

class weld {
public:
	weld();
	~weld();

	wxInt32 GetNumDef();
	void Filling();		//���������� ��� ��������� � ����������� � ��������� �� ���������
	void Sort(wxVector<defect> &vector);
	bool Intersect(const wxVector<defect> &vector);
	wxString GetHeight();
	wxString GetWidth();
	wxString GetDisplacement();
	wxString Print();
	wxString GetParamStr();

	wxInt32 objectID = 0;	//0-����������� 1-������1 2-������2
	wxInt32 weldID = 1;		//0-���������� 1-�������� 2-�������
	wxInt32 methodID = 0;	//0-��� 1-��� 2-�� 3-��

	wxInt32 weldLength = 3352;
	wxInt32 objectThickness = 12;
	wxInt32 sumDefLength = 0;
	wxInt32 minWeldWidth = 0;
	wxInt32 maxWeldWidth = 0;


private:
	///�������� double �� ���������� ����� ����� �������
	wxString Clipping(wxDouble num, wxInt32 sign = 1);

	std::mt19937_64 gen;
	defect *def;
	wxVector<defect> vectorDef;

	wxFloat32 minWeldHeight = 0.f;
	wxFloat32 maxWeldHeight = 0.f;
	wxFloat32 weldDisplacement = 0.f;
};

