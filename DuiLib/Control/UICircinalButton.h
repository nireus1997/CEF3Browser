#pragma once
#ifndef __UICIRCINALBUTTON_H__
#define __UICIRCINALBUTTON_H__

namespace DuiLib
{

	class UILIB_API CCircinalButtonUI : public CButtonUI
	{
	public:
		CCircinalButtonUI();

		LPCTSTR GetClassName() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintBkImage(HDC hDC);
		void SetBkImage(LPCTSTR pStrImage);

		void SetDefaultBkImage(LPCTSTR pStrImage) { m_sDefaultBkImage = pStrImage; }
		CDuiString GetDefaultBkImage() { return m_sDefaultBkImage; }

		void SetPenColor(DWORD dwColor) { m_dwPenColor = dwColor; }
		DWORD GetPenColor(HDC hdc);

		void SetPenWidth(int iPenWidth) { m_iPenWidth = iPenWidth; }
		int GetPenWidth()	{ return m_iPenWidth; }

		bool IsImageExist(LPCTSTR pStrImage);

	private:
		CDuiString m_sDefaultBkImage;
		DWORD m_dwPenColor;
		int m_iPenWidth;

	};



}
#endif // __UICIRCINALBUTTON_H__