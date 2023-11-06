#pragma once
#include ".\interface\cdialog.h"
#include ".\interface\cButton.h"

class CWeatherDialog :
	public cDialog
{

	cButton*		m_pWeatherBtn[eWS_Max];

public:
	CWeatherDialog(void);
	virtual ~CWeatherDialog(void);

	void Linking();
	//virtual void SetActive(BOOL val);

	void SetWeather(WORD wState);
};
