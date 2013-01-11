
#pragma once


#include "Lib/DiaWrapper.h"

// class CPropertiesToolBar : public CMFCToolBar
// {
// public:
// 	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
// 	{
// 		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
// 	}
// 
// 	virtual BOOL AllowShowOnList() const { return FALSE; }
// };



class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();

protected:
	typedef struct _SPropItem
	{
		CMFCPropertyGridProperty *prop;
	} SPropItem;
	typedef std::list<SPropItem*> PropList;
	typedef PropList::iterator PropItor;

	PropList				m_PropList;
	CFont					m_fntPropList;
//	CComboBox m_wndObjectCombo;
//	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl	m_wndPropList;

// 구현입니다.
public:
	void	UpdateProperty(const CString &symbolTypeName);
	void	AdjustLayout();
	void	SetVSDotNetLook(BOOL bSet);

protected:
	void	MakeProperty(CMFCPropertyGridProperty *pParentProp, SSymbolInfo &symbol);
	void    MakeUDT(CMFCPropertyGridProperty *pParentProp, SSymbolInfo &symbol);
	void	MakeData(CMFCPropertyGridProperty *pParentProp, SSymbolInfo &symbol);

	void	AddProperty(CMFCPropertyGridProperty *prop);
	void	DeleteAllProperty();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

