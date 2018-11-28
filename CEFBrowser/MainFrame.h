#pragma once
#include "WindowShadow.h"
#include <shellapi.h>
class CEventManage;
class CMainFrame : public WindowImplBase
{
public:
	CMainFrame();
private:
	~CMainFrame();

public:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;

public:
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnVisibleChanged(UINT, WPARAM, LPARAM, BOOL& bHandled);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	bool InstallIcon(HINSTANCE hInstance, HICON hIcon, UINT nID);
	bool UnInstallIcon();
	LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMenuMessage(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

public:
	//确保所有浏览器标签都被关闭了
	bool CheckIfAllBrowserClosed();
private:
	WindowShadow* m_wShadow;
	HMENU m_NotifyMenu;
	NOTIFYICONDATA m_nid;

	map<CDuiString, bool> m_MenuCheckInfo; //保存菜单的单选复选信息
private:
	CEventManage* m_pEventManage;
};
