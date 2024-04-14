#include "myFrame.h"

myFrame::myFrame()
	: wxFrame(NULL, wxID_ANY, "DefGen v1.0",
		wxPoint(wxDefaultPosition), wxSize(400, 600),
		wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {

	wxMenu* menuFile = new wxMenu;				//������ ���� "����"
	menuFile->Append(ID_Hello, "������", "��� �����?");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "&�����", "�����.");

	wxMenu* menuHelp = new wxMenu;				//������ ���� "������"
	menuHelp->Append(wxID_ABOUT, "����������� �����?", "����� ��� �����?");	//����� ���� "� ���������"

	wxMenuBar* menuBar = new wxMenuBar;			//�������� ������ ����
	menuBar->Append(menuFile, "&����");	//���������� ������� �� ������ ����
	menuBar->Append(menuHelp, "&������");

	widgets = new widget(this);		//�������� ���� �������� � ���� (this) ����

	SetMenuBar(menuBar);						//��������� ������ ����

	CreateStatusBar();							//�������� ������ �������
	SetStatusText("���, �� ����� �������� ���� ����!");

	Bind(wxEVT_MENU, &myFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &myFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &myFrame::OnExit, this, wxID_EXIT);
}

void myFrame::OnExit(wxCommandEvent&) {
	Close(true);
}

void myFrame::OnAbout(wxCommandEvent&) {
	wxMessageBox("\n\n��� ��������� ������������� ��� ���������"
				"\n� ������� ���������� ���������� �������� �� I ���������"
				"\n� ������������ � ��-25.160.10-���-016-15 � ���. 1,2.",
				"����������", wxOK | wxICON_INFORMATION);
}

void myFrame::OnHello(wxCommandEvent&) {
	wxMessageBox("����� ��� AV26",
		"������ ����������...", wxOK | wxICON_INFORMATION);
}
