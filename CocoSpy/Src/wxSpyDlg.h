#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/event.h>
#include <wx/clipbrd.h>

#include "SpyHelper.h"

USING_NS_CC;

class CwxSpyDlg : public wxDialog
{
protected:
	wxPropertyGridManager* m_pgMgr;
	wxPGProperty* m_pgProp;
	wxPGProperty* m_pgPropW;
	wxPGProperty* m_pgPropH;
	wxTreeCtrl* m_treeCtrl;
	wxButton* m_refProbutton;
	wxListCtrl* m_listCtrl;
	wxButton* m_button13;
	wxButton* m_button15;
	wxButton* m_button17;
	wxMenu*	m_pProMenuPopup;
	wxTreeItemId visitItem;

	CCNode* m_curProNode;
	CCScene* m_pLastScene;
	typedef map<CCNode*,wxTreeItemId> TREEITEMMAPNODE;
	TREEITEMMAPNODE m_nodeTreeItem;
	vector<CCNode*> m_vecNode;
protected:
	virtual void onPropertyValueChanged(wxPropertyGridEvent& event);
	virtual void onPropertyRightClick(wxPropertyGridEvent& event);
	virtual void onTreeSelChanged(wxTreeEvent& event);
	virtual void onTreeItemRightClick(wxTreeEvent& event);
	virtual void onCtrlListItemSelected(wxListEvent& event);
	virtual void onRefreshClick(wxCommandEvent& event);

	void OnPorpertyCopy(wxCommandEvent& WXUNUSED(event));
	void OnPropertyPaste(wxCommandEvent& WXUNUSED(event));
protected:
	wxPGProperty* getPGProperty(string name,Json::Value& jObject);
	void addNodeItemToTree(wxTreeItemId idParent, CCNode* prootNode);
	void onSetNodePropertyValue(Json::Value& jsonroot);
	void setSelectOnTree(CCNode* pNode);
public:
	CwxSpyDlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("spy++"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(650,650), long style = wxDEFAULT_DIALOG_STYLE|/*wxSTAY_ON_TOP|*/wxRESIZE_BORDER);
	virtual ~CwxSpyDlg();

	CCScene* getLastScene();
	void setLastScene(CCScene* pScene);
	void refreshTree(CCScene* prootNode);
	void refreshCtrlList(vector<CCNode*>& vecNode);
	void refreshProperty(CCNode *pNode);
	void refreshNodeDestroy(CCNode* destroyNode);
//	DECLARE_DYNAMIC_CLASS(CMainDlg)
	DECLARE_EVENT_TABLE()
};

//DECLARE_APP(CMainDlg)