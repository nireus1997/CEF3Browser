#pragma once
class CCEFTempWnd : public CWindowWnd
{
public:
	CCEFTempWnd();
	~CCEFTempWnd();

public:
	void Init();

public:
	LPCTSTR GetWindowClassName() const;
};
