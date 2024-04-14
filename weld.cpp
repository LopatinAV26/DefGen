#include <wx/log.h>
#include "weld.h"

weld::weld() {
	auto now = std::chrono::high_resolution_clock::now();
	gen.seed(now.time_since_epoch().count());

	def = new defect;
}

weld::~weld() {
	delete def;
}

wxInt32 weld::GetNumDef() {
	wxInt32 minNumDefects = static_cast<wxInt32>(weldLength / 800);
	if (minNumDefects < 1) {
		minNumDefects = 1;
	}
	wxInt32 maxNumDefects = static_cast<wxInt32>(weldLength / 200);
	if (maxNumDefects < 1) {
		maxNumDefects = 1;
	}
	std::uniform_int_distribution<> uidNumDefects(minNumDefects, maxNumDefects);
	return uidNumDefects(gen);
}

void weld::Filling() {
	wxInt32 attempt = 0;
	do {
		vectorDef.clear();
		wxInt32 numDef = GetNumDef();
		sumDefLength = 0;
		for (size_t i = 0; i < numDef; ++i) {
			def->Set(methodID, objectID, weldLength, objectThickness);
			vectorDef.push_back(*def);
			sumDefLength += def->length;
		}
		Sort(vectorDef);
		attempt++;
	} while ((Intersect(vectorDef) == true) && (attempt < 100));	// Максимальное количество попыток заполнения
}

void weld::Sort(wxVector<defect> &vector) {
	size_t vs = vector.size();
	for (size_t i = 0; i < vs; ++i) {
		for (size_t j = 0; j < vs - 1; ++j) {
			if (vector[j].coord > vector[j + 1].coord) {
				defect temp = vector[j];
				vector[j] = vector[j + 1];
				vector[j + 1] = temp;
			}
		}
	}
}

bool weld::Intersect(const wxVector<defect> &vector) {
	size_t vs = vector.size();
	if (vs > 1) {
		for (size_t i = 0; i < vs - 1; ++i) {
			if (vector[i].coord + vector[i].length * 2 >= vector[i + 1].coord) {
				return true;
			}
		}
	}
	return false;
}

wxString weld::GetHeight() {
	if (weldID == 0 && objectThickness <= 10) {		//Продольный S<=10мм
		minWeldHeight = 1.f;
		maxWeldHeight = 2.5f;
	}
	else if (weldID == 0 && objectThickness > 10) {	//Продольный S>10мм
		minWeldHeight = 1.f;
		maxWeldHeight = 3.f;
	}
	else {												//Кольцевой
		minWeldHeight = 0.5f;
		maxWeldHeight = 3.f;
	}
	if (weldID == 2) {
		minWeldHeight = -1.0f;
		maxWeldHeight = 2.0f;
	}
	std::uniform_real_distribution<> uidMinHeight(minWeldHeight, maxWeldHeight - 1.f);	//1.f минимальная разница минимальной и максимальной ширины
	wxFloat32 min = uidMinHeight(gen);
	std::uniform_real_distribution<> uidMaxHeight(min, maxWeldHeight);
	wxFloat32 max = uidMaxHeight(gen);

	wxString minStr = std::to_string(min);
	size_t pointMin = minStr.find('.');
	minStr.erase(minStr.begin() + pointMin + 1 + 1, minStr.end());

	wxString maxStr = std::to_string(max);
	size_t pointMax = maxStr.find('.');
	maxStr.erase(maxStr.begin() + pointMax + 1 + 1, maxStr.end());

	return minStr + "/" + maxStr;
}

wxString weld::GetWidth() {
	if (weldID == 2)
		return wxEmptyString;

	std::uniform_int_distribution<> uidMinWidth(minWeldWidth, maxWeldWidth);
	wxInt32 min = uidMinWidth(gen);
	std::uniform_int_distribution<> uidMaxWidth(min, maxWeldWidth);
	wxInt32 max = uidMaxWidth(gen);

	return std::to_string(min) + "/" + std::to_string(max);
}

wxString weld::GetDisplacement() {
	if (weldID == 2)
		return wxEmptyString;

	if (objectThickness >= 10) {
		weldDisplacement = objectThickness * 0.2f;
		(weldDisplacement <= 3.0f) ? weldDisplacement : weldDisplacement = 3.0f;
	}
	else {
		weldDisplacement = objectThickness * 0.25f;
		(weldDisplacement <= 2.0f) ? weldDisplacement : weldDisplacement = 2.0f;
	}
	std::uniform_real_distribution<>uidDis(0, weldDisplacement);
	wxString strDis = std::to_string(uidDis(gen));
	std::size_t pointMax = strDis.find('.');
	strDis.erase(strDis.begin() + pointMax + 1 + 1, strDis.end());

	return strDis;
}

wxString weld::Print() {
	wxString result;
	for (auto &iter : vectorDef) {
		result += (iter.DefectString(methodID, weldID));
	}
	return result;
}

wxString weld::GetParamStr() {
	switch (methodID) {
	case 0:
		if (weldID == 2)
			return  "Высота шва: " + Clipping(minWeldHeight) + "..." + Clipping(maxWeldHeight) + "\n"
			"Fc: не допускается";
		else
			return  "Высота шва: " + Clipping(minWeldHeight) + "..." + Clipping(maxWeldHeight) + "\n"
			"Ширина шва: " + std::to_string(minWeldWidth) + "..." + std::to_string(maxWeldWidth) + "\n"
			"Максимальное смещение: " + Clipping(weldDisplacement) + "\n"
			"Fc: Lmax = " + std::to_string(def->FcL) + ", Hmax = " + Clipping(def->FcD);
		break;
	case 1:return wxEmptyString;
		break;
	case 2:
		if (objectID == 0) {
			return "SH: Lmax = " + std::to_string(def->shL) + "\n"
				"LS: Lmax = " + std::to_string(def->lsL) + ", Ymax < " + Clipping(def->lsD) + "\n"
				"LB: Lmax = " + std::to_string(def->lbL) + ", Ymax >= " + Clipping(def->lsD) + "\n"
				"CC: Lmax = " + std::to_string(def->ccL);
		}
		else {
			return "SH: Lmax = " + std::to_string(def->shL) + "\n"
				"CC: Lmax = " + std::to_string(def->ccL);
		}
		break;
	case 3:return wxEmptyString;
		break;
	default: wxLogError("Неожиданное значение defectID");
		return wxEmptyString;
		break;
	}
}

wxString weld::Clipping(wxDouble num, wxInt32 sign) {
	std::string fstr = std::to_string(num);
	size_t point = fstr.find('.');
	if (sign > 3) {
		sign = 3;
	}
	fstr.erase(fstr.begin() + point + 1 + sign, fstr.end());
	return fstr;
}
