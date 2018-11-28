#include "stdafx.h"
#include "CefIconDown.h"
#include "CefUtils.h"
#include "CEFBrowserUI.h"
#include "CefClientHandler.h"

#include <wininet.h>
#include <string>
using namespace std;
#pragma comment(lib, "wininet.lib")
#define BUF_SIZE 1024

void GetInterNetURLText(LPCTSTR lpcInterNetURL)
{
	HINTERNET hSession;
	hSession = InternetOpen(_T("WinInet"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hRequest;
		hRequest = InternetOpenUrl(hSession, lpcInterNetURL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (hRequest != NULL)
		{
			DWORD dwBytesRead = 1;

			while (dwBytesRead > 0)
			{
				char szBuffer[BUF_SIZE] = { 0 };
				InternetReadFile(hRequest, szBuffer, BUF_SIZE, &dwBytesRead);
			}
		}
		InternetCloseHandle(hRequest);
	}
	InternetCloseHandle(hSession);

	return;
}

void CCefIconDownCallBack::OnDownloadImageFinished(const CefString& image_url, int http_status_code, CefRefPtr<CefImage> image)
{
	if (nullptr == image){
		GetInterNetURLText(Cef2Dui(image_url));
		return;
	}

	CPaintManagerUI* pPaintManager = m_pBrowserUI->GetManager();
	int i, j = 0;

	CefRefPtr<CefBinaryValue> value = image->GetAsPNG(1.0f, false, i, j);
	size_t data_size = value->GetSize();

	unsigned char *buff = (unsigned char*)malloc(data_size);

	value->GetData(buff, data_size, 0U);

	HBITMAP img = CRenderEngine::LoadImage(buff, data_size); 
	BITMAP bmp;

	GetObject(img, sizeof(BITMAP), &bmp);
	pPaintManager->RemoveImage(Cef2Dui(image_url));
	pPaintManager->AddImage(Cef2Dui(image_url), img, bmp.bmWidth, bmp.bmHeight, true);

	CControlUI* pIcon = m_pBrowserUI->GetIcon();
	if (nullptr == pIcon) return;

	pIcon->SetBkImage(Cef2Dui(image_url));
}
