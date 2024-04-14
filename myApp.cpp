#include "myApp.h"

bool myApp::OnInit() {
	if (!wxApp::OnInit())
		return false;
	myFrame* frame = new myFrame();
	frame->Show();
	return true;						//при возврате true запускается цикл обработки сообщений
}
