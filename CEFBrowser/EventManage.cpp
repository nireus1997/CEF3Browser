#include "stdafx.h"
#include "EventManage.h"
#include "CEFBrowserUI.h"
#include "CefClientHandler.h"
#include "BrowserManage.h"
#include "CefUtils.h"
#include "include/cef_browser.h"
#include "UIDefine.h"
enum enum_Tag_item_Pos
{
	ENUM_OPTION = 0,
	ENUM_ICON,
	ENUM_CHOSE_BTN,
};

CEventManage::CEventManage()
{

}

CEventManage::~CEventManage()
{

}

void CEventManage::Init(CPaintManagerUI* pPaintManager)
{
	CBrowserManage::GetInstance()->SetSearchPage(this);

	m_pHLayoutTitleBar = static_cast<CHorizontalLayoutUI*>(pPaintManager->FindControl(HLAYOUT_TITLEBAR_TAB));
	m_pTabBrowser = static_cast<CTabLayoutUI*>(pPaintManager->FindControl(TAB_BROWSER));
	m_pBtnNewLabel = static_cast<CButtonUI*>(pPaintManager->FindControl(BTN_NEW_LABEL));

	m_pBtnGoBack = static_cast<CButtonUI*>(pPaintManager->FindControl(BTN_GO_BACK));
	m_pBtnGoForward = static_cast<CButtonUI*>(pPaintManager->FindControl(BTN_GO_FORWARD));
	m_pBtnReload = static_cast<CButtonUI*>(pPaintManager->FindControl(BTN_RELAOD));
	m_pEditUrl = static_cast<CEditUI*>(pPaintManager->FindControl(EDIT_URL));
}

void CEventManage::Notify(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (sCtrlName.Find(_T("CloseTabBtn")) != -1)
		{
			CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
			DelBrowserLabel(pButton);
			UpdataUrlAddress();
		}
		else if (sCtrlName == BTN_GO_BACK)
		{
			OnClickBtnGoBack();
		}
		else if (sCtrlName == BTN_GO_FORWARD)
		{
			OnClickBtnGoForward();
		}
		else if (sCtrlName == BTN_RELAOD)
		{
			OnClickBtnReload();
		}
		else if (sCtrlName == BTN_NEW_LABEL)
		{
			OnClickBtnNewLabel();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (sCtrlName.Find(_T("BrowserOption")) != -1)
		{
			COptionUI* pOption = static_cast<COptionUI *>(msg.pSender);
			CHorizontalLayoutUI* pHLayout = static_cast<CHorizontalLayoutUI *>(pOption->GetParent());
			COptionTag* pTag = (COptionTag *)(pOption->GetTag());
			int nBrowserID = pTag->m_nBrowserID;

			CCEFBrowserUI* pBrowser = GetSpecifiedBrowser(nBrowserID);
			int nIndex = m_pTabBrowser->GetItemIndex(pBrowser);
			m_pTabBrowser->SelectItem(nIndex);

			UpdataUrlAddress();
			UpdataGoBackorForwardStatus();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_RETURN)
	{
		OnEnterKeyDown();
	}
}

void CEventManage::HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nBrowserID = int(wParam);
	switch (uMsg)
	{
	case CEF_ON_LOAD_START:
		break;
	case CEF_ON_LOAD_END:
		break;
	case CEF_ON_TITLE_CHANGE:{
		CefString* Title = (CefString *)(lParam);
		std::wstring str = *Title;
		str = (*Title).ToWString();

		SetTitle(nBrowserID, str.c_str());
		UpdataGoBackorForwardStatus();

		delete Title;
		break;
	}
	case CEF_ON_BEFORE_POPUP:{
		CefString* targetURL = (CefString *)(lParam);
		NewTabBrowserPage(*targetURL);
		m_pEditUrl->SetText(Cef2Dui(*targetURL));

		delete targetURL;
		break;
	}
	case CEF_ON_AFETER_CREATE:{
		 NewBrowserLabel(nBrowserID);
		 break;
	}
	case CEF_ON_URL_CHANGE:{
		CefString* targetURL = (CefString *)(lParam);
		m_pEditUrl->SetText(Cef2Dui(*targetURL));
		UpdataGoBackorForwardStatus();

		delete targetURL;
		break;
	}
	case CEF_DO_CLOSE:
		break;
	case CEF_ON_LOGIN_SUCCESS:
		HideLoginSuccessBrowser(nBrowserID);
		break;
	case CEF_ON_BEFORE_CLOSE:
		DelTabBrowserPage(nBrowserID);
		break;
	default:
		break;
	}
}

void CEventManage::NewTabBrowserPage(CefString& targetURL)
{
	CCEFBrowserUI* pBrowserUI = new CCEFBrowserUI();
	int i = m_pTabBrowser->GetCount();
	pBrowserUI->SetName(COMMON_NAME_BROWSER_SEARCH);
	std::wstring str = targetURL;
	str = targetURL.ToWString();

	pBrowserUI->m_sURL = str.c_str();
	int nCnt = m_pTabBrowser->GetCount();
	m_pTabBrowser->AddAt(pBrowserUI, nCnt);
}

void CEventManage::NewBrowserLabel(int nBrowserID)
{
	//创建一个新的标签。
	int nCnt = m_pHLayoutTitleBar->GetCount();

	//OptionUI
	COptionUI* pNewOption = new COptionUI;
	pNewOption->ApplyAttributeList(STYLE_TITLEBAR_OPTION);
	//公共标签项

	//关闭按钮
	CButtonUI* pCloseBtn = new CButtonUI;
	pCloseBtn->ApplyAttributeList(STYLE_TITLEBAR_BUTTON);

	//图标控件
	CControlUI* pIcon = new CControlUI;
	pIcon->ApplyAttributeList(STYLE_TITLEBAR_ICON);

	//标签HorLayout  ==  OptionUI + 关闭按钮
	CHorizontalLayoutUI* pNewHorLayout = new CHorizontalLayoutUI;
	pNewHorLayout->ApplyAttributeList(STYLE_TITLEBAR_HLAYOUT);

	//在标签HorLayout上增加 OptionUI 和 关闭按钮
	pNewHorLayout->Add(pNewOption);
	pNewHorLayout->Add(pIcon);
	pNewHorLayout->Add(pCloseBtn);

	m_pHLayoutTitleBar->AddAt(pNewHorLayout, nCnt - 2);


	//建立1v1关联
	COptionTag* pTag = new COptionTag(nBrowserID, nCnt - 2);
	pNewOption->SetTag((UINT_PTR)pTag);
	CCEFBrowserUI* pBrowser = GetSpecifiedBrowser(nBrowserID);

	//选中该标签
	pNewOption->Selected(true);
	CDuiString title = pBrowser->GetBrowserTitle();
	pNewOption->SetText(title);


	CCEFBrowserUI* pBrowserUI = GetSpecifiedBrowser(nBrowserID);
	//启动删除浏览器流程
	pBrowserUI->SetIcon(pIcon);
}

void CEventManage::DelTabBrowserPage(int nBrowserID)
{
	int iCnt = m_pTabBrowser->GetCount();

	CCEFBrowserUI* pBrowser = GetSpecifiedBrowser(nBrowserID);
	m_pTabBrowser->Remove(pBrowser);
}

void CEventManage::DelBrowserLabel(CButtonUI* pCloseBtn)
{
	CHorizontalLayoutUI* pHLayoutTitleBarItem = static_cast<CHorizontalLayoutUI*>(pCloseBtn->GetParent());
	CHorizontalLayoutUI* pHLayoutTitleBar = static_cast<CHorizontalLayoutUI*>(pHLayoutTitleBarItem->GetParent());
	COptionUI* pOption = static_cast<COptionUI*>(pHLayoutTitleBarItem->GetItemAt(enum_Tag_item_Pos::ENUM_OPTION));
	COptionTag* pTag = (COptionTag*)(pOption->GetTag());
	int nBrowserID = pTag->m_nBrowserID;
	int nIndexTB = pTag->m_nIndexID;
	int nBrowserIndex = pTag->m_nIndexID - 1;

	CCEFBrowserUI* pBrowser = GetSpecifiedBrowser(nBrowserID);
	pBrowser->DelBrowserPage();

	pHLayoutTitleBar->Remove(pHLayoutTitleBarItem);
	pHLayoutTitleBar->NeedParentUpdate();

	int nCnt = pHLayoutTitleBar->GetCount();
	int nCntww = m_pTabBrowser->GetCount();
	if (nCnt <= 3) return;
	CHorizontalLayoutUI* pHLayoutTitleBarItemLast = static_cast<CHorizontalLayoutUI*>(pHLayoutTitleBar->GetItemAt(nCnt - 3));
	COptionUI* pOptionT = static_cast<COptionUI*>(pHLayoutTitleBarItemLast->GetItemAt(enum_Tag_item_Pos::ENUM_OPTION));
	pOptionT->Selected(true);

}

void CEventManage::HideLoginSuccessBrowser(int nBrowserID)
{
	CCEFBrowserUI* pBrowser = GetSpecifiedBrowser(nBrowserID);
	pBrowser->SetVisible(false);

	int nOptionCnt = m_pHLayoutTitleBar->GetCount();
	for (int i = 0; i < nOptionCnt - 2; i++)
	{
		CHorizontalLayoutUI* pHLayoutTBItem = static_cast<CHorizontalLayoutUI*>(m_pHLayoutTitleBar->GetItemAt(i));
		COptionUI* pOption = static_cast<COptionUI *>(pHLayoutTBItem->GetItemAt(enum_Tag_item_Pos::ENUM_OPTION));


		COptionTag* pTag = (COptionTag *)(pOption->GetTag());
		if (nBrowserID == pTag->m_nBrowserID)
		{
			m_pHLayoutTitleBar->RemoveAt(i);
		}
	}

	m_pTabBrowser->SelectItem(0);
}

void CEventManage::SetTitle(int nBrowserID, CDuiString sTitle)
{
	int nOptionCnt = m_pHLayoutTitleBar->GetCount();
	for (int i = 0; i < nOptionCnt - 2; i++)
	{
		CHorizontalLayoutUI* pHLayoutTBItem = static_cast<CHorizontalLayoutUI*>(m_pHLayoutTitleBar->GetItemAt(i));
		COptionUI* pOption = static_cast<COptionUI *>(pHLayoutTBItem->GetItemAt(enum_Tag_item_Pos::ENUM_OPTION));

		COptionTag* pTag = (COptionTag *)(pOption->GetTag());
		if (nBrowserID == pTag->m_nBrowserID)
		{
			pOption->SetText(sTitle);
		}
	}
}

CCEFBrowserUI* CEventManage::GetSpecifiedBrowser(int nBrowserID)
{
	int nBrowserCnt = m_pTabBrowser->GetCount();

	for (int i = 0; i < nBrowserCnt; i++)
	{
		CCEFBrowserUI* pBrowser = static_cast<CCEFBrowserUI*>(m_pTabBrowser->GetItemAt(i));
		if (nBrowserID == pBrowser->m_handler->GetBrowserID())
		{
			return pBrowser;
		}
	}

	return NULL;
}

CefRefPtr<CefBrowser> CEventManage::GetCurSelBrowser()
{
	if (2 >= m_pHLayoutTitleBar->GetCount()) return nullptr;

	for (int i = 0; i < m_pHLayoutTitleBar->GetCount() - 2; i++)
	{
		CHorizontalLayoutUI* pTitleItem = static_cast<CHorizontalLayoutUI *>(m_pHLayoutTitleBar->GetItemAt(i));
		COptionUI* pOption = static_cast<COptionUI *>(pTitleItem->GetItemAt(enum_Tag_item_Pos::ENUM_OPTION));

		if (pOption->IsSelected())
		{
			COptionTag* pTag = (COptionTag *)(pOption->GetTag());
			int nBrowserID = pTag->m_nBrowserID;

			CCEFBrowserUI* pBrowserUI = GetSpecifiedBrowser(nBrowserID);
			return pBrowserUI->m_handler->GetBrowser();
		}

	}
	return nullptr;
}

void CEventManage::OnClickBtnNewLabel()
{
	CefString URL = L"http:\\www.websec.site";
	NewTabBrowserPage(URL);

	m_pEditUrl->SetText(_T("www.websec.site"));
}

void CEventManage::UpdataGoBackorForwardStatus()
{
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser)
	{
		m_pBtnGoBack->SetEnabled(false);
		m_pBtnGoForward->SetEnabled(false);
		return;
	}
	pBrowser->CanGoBack() ? m_pBtnGoBack->SetEnabled(true) : m_pBtnGoBack->SetEnabled(false);
	pBrowser->CanGoForward() ? m_pBtnGoForward->SetEnabled(true) : m_pBtnGoForward->SetEnabled(false);
}

void CEventManage::OnClickBtnGoBack()
{
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser) return;

	if (pBrowser->CanGoBack())
		pBrowser->GoBack();
}

void CEventManage::OnClickBtnGoForward()
{
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser) return;

	if (pBrowser->CanGoForward())
		pBrowser->GoForward();
}

void CEventManage::OnClickBtnReload()
{
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser) return;

	pBrowser->Reload();
}

void CEventManage::OnEnterKeyDown()
{
	if (!m_pEditUrl->IsFocused()) return;
	
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser) return;

	CDuiString pUrl = m_pEditUrl->GetText();
	if (0 == pUrl.GetLength()) return;
	pBrowser->GetMainFrame()->LoadURL(Dui2Cef(pUrl));
}

void CEventManage::UpdataUrlAddress()
{
	CefRefPtr<CefBrowser> pBrowser = GetCurSelBrowser();
	if (nullptr == pBrowser)
	{
		m_pEditUrl->SetText(_T(""));
		return;
	}
	CefString url = pBrowser->GetMainFrame()->GetURL();
	m_pEditUrl->SetText(Cef2Dui(url));
}
