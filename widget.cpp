#include "widget.h"

widget::widget(wxFrame* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(wxDefaultPosition)) {

	myWeld = new weld;

	database& db = database::Instance();
	db.SQLiteOpen("data.db");
	chObject.Add(db.SQLiteStrSelect("tbVMC", "Object", 0));
	chObject.Add(db.SQLiteStrSelect("tbVMC", "Object", 1));
	chObject.Add(db.SQLiteStrSelect("tbVMC", "Object", 2));
	db.SQLiteClose();

	object = new wxRadioBox(this, ID_ObjectBox,
		"Объект контроля", wxPoint(10, 10), wxSize(wxDefaultSize), chObject, 1);
	object->SetSelection(myWeld->objectID);	//Начальное значение объекта

	chWeldType.Add("Продольный стыковой");
	chWeldType.Add("Кольцевой стыковой");
	chWeldType.Add("Кольцевой угловой");
	weldType = new wxRadioBox(this, ID_WeldTypeBox,
		"Тип сварного шва", wxPoint(10, 110), wxSize(wxDefaultSize), chWeldType, 1);
	weldType->SetSelection(myWeld->weldID);	//Начальное значение типа шва
	weldType->Enable(0, false);	//Продольный и угловой..
	weldType->Enable(2, false);	//не активны

	chMethod.Add("ВИК");
	chMethod.Add("ПВК");
	chMethod.Add("УК");
	chMethod.Add("РК");
	method = new wxRadioBox(this, ID_MethodBox,
		"Метод контроля", wxPoint(10, 210), wxSize(wxDefaultSize), chMethod, 1);
	method->SetSelection(myWeld->methodID);	//Начальное значение метода

	thickText = new wxStaticText(this, wxID_ANY,
		"Толщина стенки", wxPoint(10, 330));
	thickSpin = new wxSpinCtrl(this, ID_ThicknessSpin, "S",
		wxPoint(12, 350), wxSize(60, 24), 16384L, 8, 17);
	thickSpin->SetValue(myWeld->objectThickness);

	diameter = new wxStaticText(this, wxID_ANY,
		"Диаметр", wxPoint(10, 380));
	diamSpin = new wxSpinCtrl(this, ID_DiameterSpin, wxEmptyString,
		wxPoint(12, 400), wxSize(60, 24), 16384L, 100, 1300, 1067);

	length = new wxStaticText(this, wxID_ANY,
		"Длина шва", wxPoint(10, 430));
	lengthSpin = new wxSpinCtrl(this, ID_LengthSpin, wxEmptyString,
		wxPoint(12, 450), wxSize(60, 24), 16384L, 200, 10000);
	lengthSpin->SetValue(myWeld->weldLength);

	length6 = new wxStaticText(this, wxID_ANY,
		wxEmptyString, wxPoint(10, 475));
	length6->SetLabel("1/6 = " + std::to_string(int(round(myWeld->weldLength / 6))));
	length6->SetForegroundColour(wxColor("Blue"));

	buttonGenerate = new wxButton(this, ID_GenButtonEvt,
		"Генерировать", wxPoint(190, 10));
	buttonGenerate->SetForegroundColour(wxColor("Red"));

	defectWindow = new wxTextCtrl(this, ID_DefectWindow,
		wxEmptyString, wxPoint(190, 40), wxSize(140, 200), wxTE_MULTILINE);

	sumLength = new wxStaticText(this, wxID_ANY,
		"Сумма всех\nдефектов", wxPoint(190, 250));
	sumWindow = new wxTextCtrl(this, ID_SumWindow,
		wxEmptyString, wxPoint(265, 255), wxSize(55, 24));

	height = new wxStaticText(this, wxID_ANY,
		"Высота\nшва", wxPoint(190, 285));
	heightWindow = new wxTextCtrl(this, ID_HeightWindow,
		wxEmptyString, wxPoint(265, 290), wxSize(55, 24));

	width = new wxStaticText(this, wxID_ANY,
		"Ширина\nшва", wxPoint(190, 320));
	widthWindow = new wxTextCtrl(this, ID_WidthWindow,
		wxEmptyString, wxPoint(265, 325), wxSize(55, 24));

	dispacement = new wxStaticText(this, wxID_ANY,
		"Величина\nсмещения", wxPoint(190, 355));
	displacementWindow = new wxTextCtrl(this, wxID_ANY,
		wxEmptyString, wxPoint(265, 360), wxSize(55, 24));

	line1 = new wxStaticLine(this, wxID_ANY, wxPoint(190, 390), wxSize(130, 1));

	params = new wxStaticText(this, wxID_ANY,
		wxEmptyString, wxPoint(190, 395));
	params->SetOwnForegroundColour(wxColor("Blue"));

	Bind(wxEVT_RADIOBOX, &widget::ObjectEvent, this, ID_ObjectBox);
	Bind(wxEVT_RADIOBOX, &widget::WeldTypeEvent, this, ID_WeldTypeBox);
	Bind(wxEVT_RADIOBOX, &widget::MethodEvent, this, ID_MethodBox);
	Bind(wxEVT_SPINCTRL, &widget::ThicknessSpinEvent, this, ID_ThicknessSpin);
	Bind(wxEVT_SPINCTRL, &widget::DiameterSpinEvent, this, ID_DiameterSpin);
	Bind(wxEVT_SPINCTRL, &widget::LengthSpinEvent, this, ID_LengthSpin);
	Bind(wxEVT_BUTTON, &widget::GenButtonEvent, this, ID_GenButtonEvt);
}

widget::~widget() {
	delete myWeld;
}

void widget::ObjectEvent(wxCommandEvent& event) {
	wxInt32 objID = myWeld->objectID = event.GetSelection();	//Получить тип шва в зависимости от объекта

	database& db = database::Instance();
	db.SQLiteOpen("data.db");
	weldType->Enable(0, db.SQLiteBoolSelect("tbWeldType", "Longitudinal", objID));
	weldType->Enable(1, db.SQLiteBoolSelect("tbWeldType", "RingWeld", objID));
	weldType->Enable(2, db.SQLiteBoolSelect("tbWeldType", "RingOverlapping", objID));

	switch (objID) {
	case 0:	//Трубопровод
		weldType->SetSelection(1);					//Переключаем на кольцевой стык
		myWeld->weldID = weldType->GetSelection();
		method->SetSelection(0);					//Переключаем на вик
		myWeld->methodID = method->GetSelection();	//Считываем новое значение метода

		method->Enable(0, true);
		method->Enable(1, true);
		method->Enable(2, true);
		method->Enable(3, true);
		break;
	case 1:	//Муфта П1
		weldType->SetSelection(0);					//Переключаем на продольный стык
		myWeld->weldID = weldType->GetSelection();
		method->SetSelection(0);					//Переключаем на вик
		myWeld->methodID = method->GetSelection();	//Считываем новое значение метода

		method->Enable(0, true);
		method->Enable(1, false);
		method->Enable(2, true);
		method->Enable(3, false);
		break;
	case 2:	//Муфта П2
		weldType->SetSelection(0);					//Переключаем на продольный стык
		myWeld->weldID = weldType->GetSelection();
		method->SetSelection(0);					//Переключаем на вик
		myWeld->methodID = method->GetSelection();	//Считываем новое значение метода

		method->Enable(0, true);
		method->Enable(1, false);
		method->Enable(2, true);
		method->Enable(3, false);
		break;
	}
	db.SQLiteClose();
}

void widget::WeldTypeEvent(wxCommandEvent& event) {
	wxInt32 objID = myWeld->objectID = object->GetSelection();
	wxInt32 wID = myWeld->weldID = weldType->GetSelection();	//Считываем новое значение типа шва
	method->SetSelection(0);					//Если меняем метод...
	myWeld->methodID = method->GetSelection();	//то считываем новое значение

	database& db = database::Instance();
	db.SQLiteOpen("data.db");

	switch (wID) {
	case 0:	//Продольный шов
		method->Enable(0, db.SQLiteBoolSelect("tbVMC", "Longitudinal", objID));
		method->Enable(1, db.SQLiteBoolSelect("tbCT", "Longitudinal", objID));
		method->Enable(2, db.SQLiteBoolSelect("tbUT", "Longitudinal", objID));
		method->Enable(3, db.SQLiteBoolSelect("tbRT", "Longitudinal", objID));
		break;
	case 1:	//Кольцевой стыковой
		method->Enable(0, db.SQLiteBoolSelect("tbVMC", "RingWeld", objID));
		method->Enable(1, db.SQLiteBoolSelect("tbCT", "RingWeld", objID));
		method->Enable(2, db.SQLiteBoolSelect("tbUT", "RingWeld", objID));
		method->Enable(3, db.SQLiteBoolSelect("tbRT", "RingWeld", objID));
		break;
	case 2:	//Кольцевой угловой
		method->Enable(0, db.SQLiteBoolSelect("tbVMC", "RingOverlapping", objID));
		method->Enable(1, db.SQLiteBoolSelect("tbCT", "RingOverlapping", objID));
		method->Enable(2, db.SQLiteBoolSelect("tbUT", "RingOverlapping", objID));
		method->Enable(3, db.SQLiteBoolSelect("tbRT", "RingOverlapping", objID));
		break;
	}
	db.SQLiteClose();
}

void widget::MethodEvent(wxCommandEvent& event) {
	myWeld->methodID = method->GetSelection();	//Считываем новое значение метода
}

void widget::ThicknessSpinEvent(wxCommandEvent& event) {
	myWeld->objectThickness = thickSpin->GetValue();
}

void widget::DiameterSpinEvent(wxCommandEvent& event) {
	myWeld->weldLength = round(diamSpin->GetValue() * M_PI);
	lengthSpin->SetValue(myWeld->weldLength);
	length6->SetLabel("1/6 = " + std::to_string(wxInt32(round(myWeld->weldLength / 6))));
}

void widget::LengthSpinEvent(wxCommandEvent& event) {
	diamSpin->SetValue(round(lengthSpin->GetValue() / M_PI));
	myWeld->weldLength = lengthSpin->GetValue();
	length6->SetLabel("1/6 = " + std::to_string(wxInt32(round(myWeld->weldLength / 6))));
}

void widget::GenButtonEvent(wxCommandEvent& event) {
	defectWindow->Clear();
	sumWindow->Clear();
	heightWindow->Clear();
	widthWindow->Clear();
	displacementWindow->Clear();
	myWeld->Filling();
	defectWindow->AppendText(myWeld->Print());
	sumWindow->AppendText(std::to_string(myWeld->sumDefLength));

	if (myWeld->methodID == 0) {
		database& db = database::Instance();
		db.SQLiteOpen("data.db");
		if (myWeld->weldID == 0) {
			myWeld->minWeldWidth = db.SQLiteIntSelect("tbWeldWidth_11_2", "WidthMin", "Thickness", myWeld->objectThickness);
			myWeld->maxWeldWidth = db.SQLiteIntSelect("tbWeldWidth_11_2", "WidthMax", "Thickness", myWeld->objectThickness);
		}
		else {
			myWeld->minWeldWidth = db.SQLiteIntSelect("tbWeldWidth_037_14", "WidthMin", "Thickness", myWeld->objectThickness);
			myWeld->maxWeldWidth = db.SQLiteIntSelect("tbWeldWidth_037_14", "WidthMax", "Thickness", myWeld->objectThickness);
		}
		db.SQLiteClose();

		heightWindow->AppendText(myWeld->GetHeight());
		widthWindow->AppendText(myWeld->GetWidth());
		displacementWindow->AppendText(myWeld->GetDisplacement());
	}
	params->SetLabel(myWeld->GetParamStr());
}
