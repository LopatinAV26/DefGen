#include <wx/log.h>
#include "database.h"
#include "defect.h"

defect::defect() {
	auto now = std::chrono::high_resolution_clock::now();
	gen.seed(now.time_since_epoch().count());
}

wxString defect::DefectString(const wxInt32 &method_ID, const wxInt32 &weldID) {
	switch (method_ID) {
	case 0:
		if (weldID == 2)
			return wxEmptyString;
		return defectType + std::to_string(coord) + "-" + std::to_string(length) + "-" + strDepthFc + "\n";
		break;
	case 1:
		return "Пока не доступно\n";
		break;
	case 2:
		return defectType + "-" + "Ад" + "-" + std::to_string(coord) + "-" + Clipping(depthUT) + "-" + std::to_string(length) + "\n";
		break;
	case 3:
		return "Пока не доступно\n";
		break;
	default:
		wxLogError("Неожиданное значение methodID");
		return "0\n";
		break;
	}
}

void defect::Set(const wxInt32 &method_ID, const wxInt32 &object_ID, const wxInt32 &weldL, const wxInt32 &weldS) {
	database &db = database::Instance();
	db.SQLiteOpen("data.db");

	if (method_ID == 0) {	//Если ВИК
		defectType = "Fc-";
		wxFloat32 minDepthFc = 0.2f;
		wxFloat32 maxDepthFc = db.SQLiteDoubleSelect("tbDefects", "FcD", "Thickness", weldS);
		FcD = maxDepthFc;
		wxInt32 minLengthFc = 3;
		wxInt32 maxLengthFc = db.SQLiteIntSelect("tbDefects", "FcL", "Thickness", weldS) - 30;
		FcL = maxLengthFc + 30;

		std::uniform_int_distribution<> uidCoord(5, weldL - 50);
		coord = uidCoord(gen);

		std::uniform_real_distribution<> uidDepth(minDepthFc, maxDepthFc);

		strDepthFc = std::to_string(uidDepth(gen));
		std::size_t pointMin = strDepthFc.find('.');
		strDepthFc.erase(strDepthFc.begin() + pointMin + 1 + 1, strDepthFc.end());

		std::uniform_int_distribution<> uidLength(minLengthFc, maxLengthFc);
		length = uidLength(gen);
	}
	else if (method_ID == 2) {	//Если УЗК
		wxInt32 defectID;
		if (object_ID != 0) {		//если не труба
			std::uniform_int_distribution<> uidType(0, 1);
			defectID = uidType(gen);
		}
		else {
			std::uniform_int_distribution<> uidType(0, 3);
			defectID = uidType(gen);
		}

		wxDouble minDepthUT = 0.0;
		wxDouble maxDepthUT = 0.0;
		wxInt32 minLengthUT = 0;
		wxInt32 maxLengthUT = 0;

		switch (defectID) {
		case 0:
			defectType = "SH";
			minDepthUT = 2.0;
			maxDepthUT = weldS - 4.0;
			minLengthUT = 3;
			maxLengthUT = db.SQLiteIntSelect("tbDefects", "shL", "Thickness", weldS) - 3;
			shL = maxLengthUT + 3;
			break;
		case 1:
			defectType = "CC";
			minDepthUT = 2.;
			maxDepthUT = weldS - 4.0;
			minLengthUT = weldS / 2;
			maxLengthUT = db.SQLiteIntSelect("tbDefects", "ccL", "Thickness", weldS) - 2;
			ccL = maxLengthUT + 2;
			break;
		case 2:
			defectType = "LS";
			minDepthUT = 0.f;
			lsD = maxDepthUT = db.SQLiteDoubleSelect("tbDefects", "lsD", "Thickness", weldS);

			minLengthUT = db.SQLiteIntSelect("tbLength", "Length", "Thickness", weldS) + 1;
			maxLengthUT = db.SQLiteIntSelect("tbDefects", "lsL", "Thickness", weldS) - 3;
			lsL = maxLengthUT + 3;
			break;
		case 3:
			defectType = "LB";
			lbD = minDepthUT = db.SQLiteDoubleSelect("tbDefects", "lbD", "Thickness", weldS);
			maxDepthUT = weldS - 1.0;
			minLengthUT = db.SQLiteIntSelect("tbLength", "Length", "Thickness", weldS) + 1;
			/////////////////////////////////////////////////////////////////////////////////
			/*Костыль для толщины стенки 10мм, т.к.в РД для причисления дефекта к типу LB,
			его минимальная длина должна быть >10мм, при этом максимальная длина
			не должна превышать толщину стенки (10мм). Поэтому для данной толщины
			стенки устанавливаем минимальную длину LB = 10мм. Второй вариант вообще исключить
			дефект LB для данной толщины стенки*/
			(weldS == 10) ? minLengthUT = 10 : minLengthUT;
			////////////////////////////////////////////////////////////////////////////////
			maxLengthUT = db.SQLiteIntSelect("tbDefects", "lbL", "Thickness", weldS);
			lbL = maxLengthUT;
			break;
		default:
			wxLogError("Неожиданное значение defectID");
			break;
		}

		std::uniform_int_distribution<> uidCoord(5, weldL - weldS * 2);
		coord = uidCoord(gen);

		std::uniform_real_distribution<> uidDepth(minDepthUT, maxDepthUT);
		depthUT = uidDepth(gen);

		std::uniform_int_distribution<> uidLength(minLengthUT, maxLengthUT);
		length = uidLength(gen);
	}
	db.SQLiteClose();
}

wxString defect::Clipping(wxDouble num, wxInt32 sign) {
	std::string fstr = std::to_string(num);
	std::size_t point = fstr.find('.');
	(sign > 3) ? sign = 3 : sign;
	fstr.erase(fstr.begin() + point + 1 + sign, fstr.end());
	return fstr;
}
