#include "myFrame.h"

myFrame::myFrame()
	: wxFrame(NULL, wxID_ANY, "DefGen v1.0",
		wxPoint(wxDefaultPosition), wxSize(400, 600),
		wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {

	wxMenu* menuFile = new wxMenu;				//список меню "файл"
	menuFile->Append(ID_Hello, "Здаров", "Как жизнь?");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "&Выход", "Пакет.");

	wxMenu* menuHelp = new wxMenu;				//список меню "помощь"
	menuHelp->Append(wxID_ABOUT, "Беспомощный штоле?", "Зачем эта шляпа?");	//пункт меню "О программе"

	wxMenuBar* menuBar = new wxMenuBar;			//создание строки меню
	menuBar->Append(menuFile, "&Файл");	//добавление пунктов на строку меню
	menuBar->Append(menuHelp, "&Помощь");

	widgets = new widget(this);		//создание моих виджетов в этом (this) окне

	SetMenuBar(menuBar);						//включение строки меню

	CreateStatusBar();							//создание строки статуса
	SetStatusText("Хай, да будет облегчен твой труд!");

	Bind(wxEVT_MENU, &myFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &myFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &myFrame::OnExit, this, wxID_EXIT);
}

void myFrame::OnExit(wxCommandEvent&) {
	Close(true);
}

void myFrame::OnAbout(wxCommandEvent&) {
	wxMessageBox("\n\nЭта программа предназначена для генерации"
				"\nв сварном соединении допустимых дефектов по I категории"
				"\nв соответствии с РД-25.160.10-КТН-016-15 с Изм. 1,2.",
				"Информация", wxOK | wxICON_INFORMATION);
}

void myFrame::OnHello(wxCommandEvent&) {
	wxMessageBox("ЗДЕСЬ БЫЛ AV26",
		"Важная информация...", wxOK | wxICON_INFORMATION);
}
