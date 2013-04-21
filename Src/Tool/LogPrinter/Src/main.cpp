
#include "Global.h"
#include <wx/aui/aui.h>
#include "Printer.h"
#include "Common/Common.h"

using namespace std;

class MyFrame : public wxFrame 
{
	enum  {
		ID_REFRESH_TIMER = 100,
		REFRESH_INTERVAL = 1000,
	};
public:
	MyFrame(wxWindow* parent);
	~MyFrame() { m_mgr.UnInit(); }
//protected:
	// Event Handler
	DECLARE_EVENT_TABLE()
	void OnRefreshTimer(wxTimerEvent& event);
//private:
public:
	wxAuiManager m_mgr;
	wxTimer	m_Timer;
	list<string> m_fileList;
	list<string> m_CmdLine;
};


BEGIN_EVENT_TABLE( MyFrame, wxFrame )
	EVT_TIMER(ID_REFRESH_TIMER, MyFrame::OnRefreshTimer)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow* parent) : wxFrame(parent, -1, _("LogPrinter"),
	wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE)
{
	// notify wxAUI which frame to use
	m_mgr.SetManagedWindow(this);

	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
		wxDefaultPosition, wxSize(200,150),
		wxNO_BORDER | wxTE_MULTILINE);
	
	m_mgr.AddPane(text3, wxCENTER);

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();

	//m_CmdLine

	m_Timer.SetOwner(this, ID_REFRESH_TIMER);
	m_Timer.Start( REFRESH_INTERVAL );
}


/**
 @brief 
 */
void MyFrame::OnRefreshTimer(wxTimerEvent& event)
{
	list<string> fileList;
	if (m_CmdLine.empty())
	{
		fileList = common::FindFileList( "*.log" );
	}
	else
	{
		auto it = m_CmdLine.begin();
		while (m_CmdLine.end() != it)
		{
			list<string> files = common::FindFileList( *it++ );
			copy(files.begin(), files.end(), back_inserter(fileList));
		}
	}
	fileList.sort();

	set<string> result;
	std::set_difference(fileList.begin(), fileList.end(), m_fileList.begin(), m_fileList.end(),
		insert_iterator<set<string> >(result, result.end()) );

	if (!result.empty())
	{
		std::copy(result.begin(), result.end(), inserter(m_fileList, m_fileList.end()));
		m_fileList.sort();

		auto it = result.begin();
		while (result.end() != it)
		{
			CPrinter *prt = new CPrinter(this, *it);
			m_mgr.AddPane(prt, wxBOTTOM, *it);
			wxAuiPaneInfo& pane = m_mgr.GetPane(prt);
			pane.MinSize(0,1000);
			it++;
		}
		m_mgr.Update();
	}
}


// our normal wxApp-derived class, as usual
class MyApp : public wxApp {
public:
	bool OnInit()
	{
		MyFrame* frame = new MyFrame(NULL);
		
		if (argc >1)
		{
			for (int i=1; i < argc; ++i)
			{
				string str = argv[ i].c_str();
				frame->m_CmdLine.push_back( str );
			}
		}

		SetTopWindow(frame);
		frame->Show();
		return true;                    
	}
};

DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);