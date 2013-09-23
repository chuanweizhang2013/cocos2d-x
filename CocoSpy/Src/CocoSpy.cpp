

#include "CocoSpy.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if wxUSE_DATEPICKCTRL
#include <wx/datectrl.h>
#endif

#include <wx/artprov.h>
#include <wx/numdlg.h>
#include "wxSpyDlg.h"
#include "SpyHelper.h"

USING_NS_CC;

static CwxSpyDlg *pSpyDlg=NULL;
void ShowDlg()
{
	if (pSpyDlg)
	{
		if(!pSpyDlg->IsVisible())
			pSpyDlg->Show();
		return;
	}

	static wxApp spyApp;
	static wxWindow win;
	wxPGInitResourceModule();
	win.Enable(true);
	win.SetHWND((WXHWND)CCEGLView::sharedOpenGLView()->getHWnd());

	pSpyDlg = new CwxSpyDlg(&win);
	pSpyDlg->Show(true);
}

void SetFocusRect(CCNode* pFocusNode)
{
	CCSprite::m_DrawBoundingBoxSprite =(CCSprite*)pFocusNode;
}

void NodeDestroyProc (CCNode* destroyNode)
{
	if(pSpyDlg)
	{
		pSpyDlg->refreshNodeDestroy(destroyNode);
	}
}

LRESULT CustomWndProc (UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed)
{
	
	switch (message)
	{
	case  WM_RBUTTONDOWN:
		{
			ShowDlg();
			
			if (pSpyDlg->getLastScene()!= CCDirector::sharedDirector()->getRunningScene())
			{
				pSpyDlg->setLastScene( CCDirector::sharedDirector()->getRunningScene());
				pSpyDlg->refreshTree(pSpyDlg->getLastScene());
			}
			POINT point = {(short)LOWORD(lParam), (short)HIWORD(lParam)};
			vector<CCNode*> vecNode;
			CSpyHelper::sharedHelper().findNodeByPoint(point.x,point.y,vecNode);
			pSpyDlg->refreshCtrlList(vecNode);

			if(vecNode.size()>0)
			{
				pSpyDlg->refreshProperty(vecNode[0]);
			}else
			{
				SetFocusRect(NULL);
			}
			//pSpyDlg->SetFocus();
		}
		break;
	default:
		break;
	}
	return message;
}
