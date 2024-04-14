#pragma once
#include <wx/wx.h>
#include "widget.h"

class myFrame : public wxFrame {
public:
	myFrame();
	
private:
	widget* widgets;

	enum ID {
		ID_Hello = 1,
	};

	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
};

