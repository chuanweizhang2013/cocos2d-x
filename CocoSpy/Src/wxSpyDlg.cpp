#include "wxSpyDlg.h"

enum
{
	Menu_Active     = 100,
	Menu_Property,
	Menu_Copy,
	Menu_Paste,
};

// IMPLEMENT_APP    (CMainDlg)
// IMPLEMENT_DYNAMIC_CLASS    (CMainDlg, wxDialog)

BEGIN_EVENT_TABLE(CwxSpyDlg, wxDialog)
	EVT_MENU(Menu_Copy,        CwxSpyDlg::OnPorpertyCopy)
	EVT_MENU(Menu_Paste,       CwxSpyDlg::OnPropertyPaste)
END_EVENT_TABLE()

extern void SetFocusRect(CCNode* pFocusNode);
extern void NodeDestroyProc (CCNode* destroyNode);

CwxSpyDlg::~CwxSpyDlg()
{
	m_pgMgr->Disconnect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(CwxSpyDlg::onPropertyValueChanged), NULL, this);
	m_pgMgr->Disconnect(wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler(CwxSpyDlg::onPropertyRightClick), NULL, this);
	m_treeCtrl->Disconnect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(CwxSpyDlg::onTreeSelChanged), NULL, this);
	m_treeCtrl->Disconnect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(CwxSpyDlg::onTreeItemRightClick), NULL, this);
	m_listCtrl->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CwxSpyDlg::onCtrlListItemSelected), NULL, this);
	m_refProbutton->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CwxSpyDlg::onRefreshClick), NULL, this);
}
CwxSpyDlg::CwxSpyDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style)
{
// 	wxFont dlgFont= GetFont();
// 	dlgFont.SetPointSize(dlgFont.GetPointSize()/4);
// 	SetOwnFont(dlgFont);
	m_curProNode = NULL;
	m_pLastScene=NULL;

	m_pProMenuPopup = new wxMenu;
	m_pProMenuPopup->Append(Menu_Copy, wxT("Copy"), wxT(""));
	m_pProMenuPopup->Append(Menu_Paste, wxT("Paste"), wxT(""));

	wxBoxSizer* boxSizer3 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(boxSizer3);

	wxBoxSizer* boxSizer5 = new wxBoxSizer(wxHORIZONTAL);

	boxSizer3->Add(boxSizer5, 1, wxALL|wxEXPAND, 5);

	wxArrayString m_pgMgrArr;
	wxUnusedVar(m_pgMgrArr);
	wxArrayInt m_pgMgrIntArr;
	wxUnusedVar(m_pgMgrIntArr);
	wxPropertyGridPage* page;
	//m_pgMgr = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxPG_DESCRIPTION|wxPG_TOOLBAR|wxPG_TOOLTIPS|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
	m_pgMgr = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxSize(280,400), wxPG_TOOLBAR|wxPG_TOOLTIPS|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
	m_pgMgr->SetToolTip(_("可以通过修改属性的值进行调试"));
	//m_pgMgr->ShowHeader();
	int extraStyle = wxPG_EX_MODE_BUTTONS |
		wxPG_EX_MULTIPLE_SELECTION;
	m_pgMgr->SetExtraStyle(extraStyle);
	m_pgMgr->AddPage(wxT("Standard Items"));

	boxSizer5->Add(m_pgMgr, 0, wxALL|wxEXPAND, 5);
	
	wxBoxSizer* boxSizer25 = new wxBoxSizer(wxVERTICAL);
	boxSizer5->Add(boxSizer25, 1, wxALL|wxEXPAND, 5);
	//m_pgMgr->SetDescBoxHeight(m_pgMgr->GetDescBoxHeight()*4/5);
// 	wxFont pgFont=m_pgMgr->GetFont();
// 	pgFont.SetPointSize(pgFont.GetPointSize()/2);
// 	m_pgMgr->SetOwnFont(pgFont);
	m_listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1,120), wxLC_REPORT);
	m_listCtrl->InsertColumn(m_listCtrl->GetColumnCount(), _("NodeName"), wxLIST_FORMAT_LEFT, m_listCtrl->GetRect().GetWidth());
// 	wxFont treeFont=m_treeCtrl->GetFont();
// 	treeFont.SetPointSize(treeFont.GetPointSize()/2);
// 	m_treeCtrl->SetOwnFont(treeFont);
	boxSizer25->Add(m_listCtrl, 0, wxALL|wxEXPAND, 5);
	m_treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTR_DEFAULT_STYLE);

	boxSizer25->Add(m_treeCtrl, 1, wxALL|wxEXPAND, 5);

	m_refProbutton = new wxButton(this, wxID_ANY, _("refresh"), wxDefaultPosition, wxSize(-1,-1), 0);
	
	boxSizer25->Add(m_refProbutton, 0, wxALL, 5);

	wxBoxSizer* boxSizer11 = new wxBoxSizer(wxHORIZONTAL);

	boxSizer3->Add(boxSizer11, 0, wxALL|wxEXPAND, 5);

	m_button13 = new wxButton(this, wxID_ANY, _("My Button"), wxDefaultPosition, wxSize(-1,-1), 0);

	boxSizer11->Add(m_button13, 0, wxALL, 5);

	m_button15 = new wxButton(this, wxID_ANY, _("My Button"), wxDefaultPosition, wxSize(-1,-1), 0);

	boxSizer11->Add(m_button15, 0, wxALL, 5);

	m_button17 = new wxButton(this, wxID_ANY, _("My Button"), wxDefaultPosition, wxSize(-1,-1), 0);

	boxSizer11->Add(m_button17, 0, wxALL, 5);

	SetSizeHints(500,400);
	if ( GetSizer() ) {
		GetSizer()->Fit(this);
	}
	Centre(wxBOTH);
	// Connect events
	m_pgMgr->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(CwxSpyDlg::onPropertyValueChanged), NULL, this);
	m_pgMgr->Connect(wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler(CwxSpyDlg::onPropertyRightClick), NULL, this);
	m_treeCtrl->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(CwxSpyDlg::onTreeSelChanged), NULL, this);
	m_treeCtrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(CwxSpyDlg::onTreeItemRightClick), NULL, this);
	m_listCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CwxSpyDlg::onCtrlListItemSelected), NULL, this);
	m_refProbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CwxSpyDlg::onRefreshClick), NULL, this);
}

void CwxSpyDlg::OnPorpertyCopy(wxCommandEvent& WXUNUSED(event))
{
	if (wxTheClipboard->Open())
	{
		wxString  strValue = m_pgMgr->GetSelectedProperty()->GetValueString();
		wxTheClipboard->SetData( new wxTextDataObject(strValue) );
		wxTheClipboard->Close();
	}

}

void CwxSpyDlg::OnPropertyPaste(wxCommandEvent& WXUNUSED(event))
{
	// Read some text
	if (wxTheClipboard->Open())
	{
		if (wxTheClipboard->IsSupported( wxDF_TEXT ))
		{
			wxTextDataObject data;
			wxTheClipboard->GetData( data );
			m_pgMgr->GetSelectedProperty()->SetValue(data.GetText());
		}  
		wxTheClipboard->Close();
	}
}

CCScene* CwxSpyDlg::getLastScene()
{
	return m_pLastScene;
}

void CwxSpyDlg::setLastScene(CCScene* pScene)
{
	m_pLastScene =pScene;
}

void CwxSpyDlg::refreshCtrlList(vector<CCNode*>& vecNode)
{
	m_listCtrl->DeleteAllItems();
	for(int i=0;i < vecNode.size();++i)
	{
		int itemCount=m_listCtrl->GetItemCount();
		string strNodeName = CSpyHelper::sharedHelper().getNodeClassName(vecNode[i]);
		m_listCtrl->InsertItem(itemCount,strNodeName.c_str());
	}
	m_vecNode=vecNode;
}

void CwxSpyDlg::refreshTree(CCScene* prootNode)
{
	CCScene *pScene= dynamic_cast<CCScene*>(prootNode);
	if (NULL==pScene)
		return;

	m_nodeTreeItem.clear();
	m_treeCtrl->DeleteAllItems();
	wxTreeItemId parentItem;
	addNodeItemToTree(parentItem,prootNode);
	m_treeCtrl->ExpandAll();
}
void CwxSpyDlg::addNodeItemToTree(wxTreeItemId parentItem, CCNode* prootNode)
{
	if (NULL==prootNode)
		return;
	
	wxTreeItemId tmpTreeItemId;
	CCLayer *pLayer= dynamic_cast<CCLayer*>(prootNode);
	CCScene *pScene= dynamic_cast<CCScene*>(prootNode);
	CCNode *pNode= dynamic_cast<CCNode*>(prootNode);
	
	if (pNode)
	//if (pLayer || pScene)
	{
		char szLayerName[128]={0};
		string strClassName=CSpyHelper::sharedHelper().getNodeClassName(prootNode);
		sprintf(szLayerName,"%s",strClassName.c_str());
		if (parentItem.IsOk())
			tmpTreeItemId= m_treeCtrl->AppendItem(parentItem, szLayerName);
		else
			tmpTreeItemId=m_treeCtrl->AddRoot(wxString(szLayerName));
		prootNode->setDestroyCallBack(NodeDestroyProc);
		m_nodeTreeItem[prootNode]=tmpTreeItemId;
	}
	CCArray* childrenArray = prootNode->getChildren();
	if (childrenArray && childrenArray->count() > 0)
	{
		CCNode *pChildNode = NULL;
		for (int i =0; i <childrenArray->count(); i++)
		{
			pChildNode = (CCNode*) childrenArray->objectAtIndex(i);
			if(!tmpTreeItemId.IsOk())
				tmpTreeItemId = parentItem;
			addNodeItemToTree(tmpTreeItemId,pChildNode);
		}
	}
}

void CwxSpyDlg::refreshNodeDestroy(CCNode* destroyNode)
{
	wxTreeItemId delItemId = m_nodeTreeItem[destroyNode];
	if(delItemId.IsOk())
	{
		m_treeCtrl->DeleteChildren(delItemId);
		m_treeCtrl->Delete(delItemId);
	}

	if(destroyNode == m_curProNode)
		m_pgMgr->GetPage(0)->Clear();

	if(m_treeCtrl->GetCount())
	{
		for(int i=0;i<m_vecNode.size();i++)
		{
			if(destroyNode == m_vecNode[i])
			{
				m_listCtrl->DeleteItem(i);
				break;
			}
		}
	}else
	{
		m_listCtrl->DeleteAllItems();
	}
}

void CwxSpyDlg::setSelectOnTree(CCNode* pNode)
{
	if(NULL == pNode)
		return;
	wxTreeItemId visitItem=m_nodeTreeItem[pNode];
	if(visitItem.IsOk())
	{
		wxString str=m_treeCtrl->GetItemText(visitItem);
		m_treeCtrl->SelectItem(visitItem);
		//m_treeCtrl->ScrollTo(visitItem);
	}
}

void CwxSpyDlg::refreshProperty(CCNode *pNode)
{
	SetFocusRect(pNode);
	if(NULL==pNode)
	{
		return;
	}
	m_curProNode = pNode;
	Json::Value jObject;
	string className=CSpyHelper::sharedHelper().getNodeClassName(pNode);
	jObject["ClassName"]=className.c_str();
	pNode->getProperties(jObject);
	
	onSetNodePropertyValue(jObject);
	setSelectOnTree(pNode);
	//SetFocusRect(pNode);
}

 wxPGProperty* CwxSpyDlg::getPGProperty(string name,Json::Value& jObject)
{
	wxPGProperty* pPro=NULL;
	if(jObject.isBool())
	{
		pPro = new wxBoolProperty( _(name),wxPG_LABEL,(bool)jObject.asBool());
	}
	else if(jObject.isInt())
	{
		pPro = new wxIntProperty( name,wxPG_LABEL,jObject.asInt());
		pPro->SetEditor(wxPGEditor_SpinCtrl);
		pPro->SetAttribute(wxT("Step"), (int)1);
	}
	else if(jObject.isUInt())
	{
		pPro = new wxUIntProperty( name,wxPG_LABEL,jObject.asUInt());
		pPro->SetEditor(wxPGEditor_SpinCtrl);
		pPro->SetAttribute(wxT("Step"), (int)1);
	}
	else if(jObject.isDouble())
	{
		pPro = new wxFloatProperty(name,wxPG_LABEL, jObject.asDouble());
		pPro->SetEditor(wxPGEditor_SpinCtrl);
		pPro->SetAttribute(wxT("Step"), (float)0.1);
	}
	else
	{
		pPro = new wxStringProperty(name,wxPG_LABEL,jObject.asString());
	}
//	else if(jObject.isArray())
// 	{
// 
// 	}else if(jObject.isObject)
// 	{
// 
// 	}

	return pPro;
}

void CwxSpyDlg::onSetNodePropertyValue(Json::Value& jsonroot)
{
	//m_pgMgr->Clear();
	m_pgMgr->GetPage(0)->Clear();

	Json::Value::Members members( jsonroot.getMemberNames() );
	for ( Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it )
	{
		const std::string name = *it;
		std::vector<std::string> vecString;
		Split(name,vecString,".");
		if(vecString.size()<=0 || TestVecStringEmpty(vecString))
			continue;

		wxPGProperty* rootNode=NULL;
		//rootNode =m_pgMgr->GetPage(0)->GetPropertyByName("CCNode");
		rootNode = m_pgMgr->GetPropertyByName(vecString[0].c_str());
		if(NULL == rootNode)
		{
			if(vecString.size()>1)
			{
				//rootNode = new wxPropertyCategory(vecString[0].c_str(),wxPG_LABEL);
				rootNode = new wxStringProperty(vecString[0].c_str(),wxPG_LABEL,wxT("<composed>"));
				rootNode->Enable(false);
				//rootNode = new wxStringProperty("CCNode",wxPG_LABEL,wxT("<composed>"));
			}
			else
			{
				rootNode=getPGProperty(name,jsonroot[name]);
				//rootNode = new wxStringProperty(jsonroot[name],wxPG_LABEL,wxT("<composed>"));
			}
			m_pgMgr->GetPage(0)->Append(rootNode);
		}
		wxPGProperty* ParentNode =rootNode;
		for(int i=1;i< vecString.size();++i)
		{
			char szProName[512]={0};
			sprintf(szProName,"%s",vecString[0].c_str());
			for(int j=1;j<= i;j++)
			{
				sprintf(szProName,"%s.%s",szProName,vecString[j].c_str());
			}
			wxPGProperty*  childNode = m_pgMgr->GetPage(0)->GetPropertyByName(szProName);
			if(NULL == childNode)
			{
				if(i==vecString.size()-1)
				{
					childNode=getPGProperty(vecString[i].c_str(),jsonroot[name]);
				}
				else
				{
					childNode = new wxStringProperty(vecString[i].c_str(),wxPG_LABEL,wxT("<composed>"));
					childNode->Enable(false);
				//childNode = new wxStringProperty(proName.c_str(),wxPG_LABEL,wxT("<composed>"));
				}
				m_pgMgr->GetPage(0)->AppendIn(ParentNode,childNode);	
			}
			ParentNode = childNode;
		}
	}

	m_pgMgr->Refresh();
}

void CwxSpyDlg::onPropertyValueChanged(wxPropertyGridEvent& event) 
{
	wxPGProperty* property = event.GetProperty();
	wxString proname = property->GetName();
	wxAny provalue = property->GetValue();
	if ( provalue.IsNull() || proname.IsEmpty())
		return;
	if (property->GetChildCount()>0)
		return;
	Json::Value jObject;
	wxStringProperty *pgStringProperty = dynamic_cast<wxStringProperty*>(property);
	if (pgStringProperty)
	{	
		jObject[proname.ToStdString().c_str()]=wxANY_AS(provalue, wxString).ToStdString().c_str();
	}
	wxBoolProperty *pgBoolCategory = dynamic_cast<wxBoolProperty*>(property);
	if(pgBoolCategory)
	{
		jObject[proname.ToStdString().c_str()]=provalue.As(static_cast<bool*>(NULL));
	}
	wxFloatProperty *pgFloatProperty = dynamic_cast<wxFloatProperty*>(property);
	if(pgFloatProperty)
	{
		jObject[proname.ToStdString().c_str()]=provalue.As(static_cast<float*>(NULL));
	}
	wxUIntProperty *pgUintProperty = dynamic_cast<wxUIntProperty*>(property);
	if(pgUintProperty)
	{
		jObject[proname.ToStdString().c_str()]=provalue.As(static_cast<unsigned int*>(NULL));
	}
	wxIntProperty *pgintProperty = dynamic_cast<wxIntProperty*>(property);
	if(pgintProperty)
	{
		jObject[proname.ToStdString().c_str()]=provalue.As(static_cast<int*>(NULL));
	}

	if(m_curProNode)
		m_curProNode->setProperties(jObject);
	SetFocusRect(m_curProNode);
}

void CwxSpyDlg::onPropertyRightClick(wxPropertyGridEvent& event)
{ 
	PopupMenu(m_pProMenuPopup);
	event.Skip(); 
}

void CwxSpyDlg::onTreeSelChanged(wxTreeEvent& event) 
{
	wxTreeItemId  foucsItem=m_treeCtrl->GetFocusedItem();
	if(!foucsItem.IsOk())
		return;

	CCNode* pFocusNode=NULL;
	TREEITEMMAPNODE::iterator it=m_nodeTreeItem.begin();
	for(;it!=m_nodeTreeItem.end();++it)
	{
		if (it->second.GetID()==foucsItem.GetID())
		{
			pFocusNode = it->first;
			break;
		}
	}
	refreshProperty(pFocusNode);
	event.Skip(); 
}

void CwxSpyDlg::onRefreshClick(wxCommandEvent& event)
{
	m_treeCtrl->DeleteAllItems();
	m_listCtrl->DeleteAllItems();
	m_pgMgr->GetPage(0)->Clear();
	m_nodeTreeItem.clear();
	m_vecNode.clear();
	wxTreeItemId visitItemtmp;
	visitItem = visitItemtmp;
	SetFocusRect(NULL);
	m_pLastScene=CCDirector::sharedDirector()->getRunningScene();
	refreshTree(m_pLastScene);
}

void CwxSpyDlg::onCtrlListItemSelected(wxListEvent& event)
{ 
	int id=event.GetItem().GetId();
	if(id < m_vecNode.size())
	{
		refreshProperty(m_vecNode[id]);
		wxTreeItemId itemId = m_nodeTreeItem[m_vecNode[id]];
		if(itemId.IsOk())
		{
			
			m_treeCtrl->SelectItem(itemId);
			m_treeCtrl->SetItemTextColour(itemId,*wxRED);
			if(visitItem.IsOk())
			{
				m_treeCtrl->SetItemTextColour(visitItem,*wxBLACK);
			}
			m_treeCtrl->ScrollTo(itemId);
			visitItem = itemId;
		}

	}
	event.Skip();
}

void CwxSpyDlg::onTreeItemRightClick(wxTreeEvent& event) 
{ 

		// select the item first
	m_treeCtrl->SelectItem(event.GetItem());
	
	//wxPoint screenpt = ClientToScreen(event.GetPoint());
	//else: take the currently selected item if click not on item
	//PopupMenu(m_pMenuPopup);
	event.Skip(); 
}
