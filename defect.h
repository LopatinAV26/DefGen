#pragma once
#include <random>
#include <chrono>
#include <cmath>
#include <wx/string.h>

class defect {
public:
	defect();

	wxString DefectString(const wxInt32 &method_ID, const wxInt32 &weldID);
	void Set(const wxInt32 &method_ID, const wxInt32 &object_ID, const wxInt32 &weldL, const wxInt32 &weldS);

	wxInt32 coord = 0;
	wxInt32 length = 0;
	wxFloat32 FcD = 0.f;
	wxInt32 FcL = 0;
	wxInt32 shL = 0, lsL = 0, lbL = 0, ccL = 0;
	wxDouble lsD = 0.f, lbD = 0.f;

private:
	wxString Clipping(wxDouble num, wxInt32 sign = 1);

	std::mt19937_64 gen;

	wxString defectType = wxEmptyString;
	wxString strDepthFc = wxEmptyString;
	wxDouble depthUT = 0.0;
};

