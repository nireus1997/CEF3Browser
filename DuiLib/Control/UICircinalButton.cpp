#include "StdAfx.h"
#include "UICircinalButton.h"
 
#include <GdiPlus.h>

namespace DuiLib
{
	CCircinalButtonUI::CCircinalButtonUI()
	{
		m_sDefaultBkImage = _T("xxx.png");

		m_dwPenColor = Color(255, 255, 255, 255).GetValue();
		m_iPenWidth = 1;
	}

	LPCTSTR CCircinalButtonUI::GetClassName() const
	{
		return _T("CircinalButtonUI");
	}

	LPVOID CCircinalButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, _T("CircinalButton")) == 0)
			return static_cast<CCircinalButtonUI*>(this);

		return CControlUI::GetInterface(pstrName);
	}

	void CCircinalButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("defaultbkimage")) == 0)
			SetDefaultBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("bkimage")) == 0)
			SetBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("pencolor")) == 0)
		{
			while (*pstrValue == _T('\0') && *pstrValue <= _T(' '))	pstrValue = ::CharNext(pstrValue);
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD dwColor = _tcstoul(pstrValue, &pstr, 16);

			SetPenColor(dwColor);
		}
		else if (_tcscmp(pstrName, _T("penwidth")) == 0)
			SetPenWidth(_ttoi(pstrValue));
		else
			return __super::SetAttribute(pstrName, pstrValue);
	}

	void CCircinalButtonUI::PaintBkImage(HDC hDC)
	{
		POINT pt = { m_rcItem.left, m_rcItem.top };

		SIZE sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };

		Graphics graphics(hDC);
		//Ïû³ý¾â³Ý
		graphics.SetSmoothingMode(SmoothingModeHighQuality);

		GraphicsPath graphicspath;
		if (graphicspath.GetLastStatus() != Ok)	return;
		graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cx);

		//ÉèÖÃ²Ã¼ôÔ²
		graphics.SetClip(&graphicspath, CombineModeReplace);

		Image image(GetBkImage());
		if (image.GetLastStatus() != Ok)	return;

		//»æÖÆÍ¼Ïñ
		graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);

		//»æÖÆÒ»¸ö1ÏñËØ¿íµÄÔ²ÐÎ£¬Ïû³ý¾â³Ý
		Pen myPen(GetPenColor(hDC), static_cast<Gdiplus::REAL>(GetPenWidth()));
		if (myPen.GetLastStatus() != Ok) return;

		graphics.DrawEllipse(&myPen, pt.x, pt.y, sz.cx, sz.cy);

		return;
	}

	void CCircinalButtonUI::SetBkImage(LPCTSTR pStrImage)
	{
		if (IsImageExist(pStrImage))
		{
			m_sBkImage = pStrImage;
		}
		else
		{
			TCHAR tszModule[MAX_PATH + 1] = { 0 };
			::GetModuleFileName(CPaintManagerUI::GetInstance(), tszModule, MAX_PATH);
			CDuiString sInstancePath = tszModule;
			int pos = sInstancePath.ReverseFind(_T('\\'));
			if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);
			sInstancePath.Append(pStrImage);
			m_sBkImage = sInstancePath;

			if (IsImageExist(sInstancePath))
			{
				m_sBkImage = sInstancePath;
			}
			else
			{
				m_sBkImage = pStrImage;
			}
		}

		Invalidate();
	}

	DWORD CCircinalButtonUI::GetPenColor(HDC hdc)
	{
		return m_dwPenColor;
	}

	bool CCircinalButtonUI::IsImageExist(LPCTSTR pStrImage)
	{
		return GetFileAttributes(pStrImage) == -1 ? false : true;
	}

}