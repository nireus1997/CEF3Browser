#include "stdafx.h"
#include "CefUtils.h"

CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs& main_args)
{
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());

	return command_line;
}

ProcessType GetProcessType(const CefRefPtr<CefCommandLine>& command_line)
{
	// The command-line flag won't be specified for the browser process.
	if (!command_line->HasSwitch(kProcessType))
		return PROCESS_TYPE_BROWSER;

	const std::string& process_type = command_line->GetSwitchValue(kProcessType);
	if (process_type == kRendererProcess)
		return PROCESS_TYPE_RENDERER;

	return PROCESS_TYPE_OTHER;
}


CefString Dui2Cef(CDuiString str)
{
	wstring url(str.GetData());
	CefString cef_str(url);
	cef_str = url;
	cef_str.FromWString(cef_str);

	return cef_str;
}

DuiLib::CDuiString Cef2Dui(CefString str)
{
	std::wstring strT = str;
	strT = str.ToWString();

	return strT.c_str();
}

