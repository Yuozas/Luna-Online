#include "stdafx.h"
#include "weatherdialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"

CWeatherDialog::CWeatherDialog(void)
{
}

CWeatherDialog::~CWeatherDialog(void)
{
}

void CWeatherDialog::Linking()
{
	int i;
	for(i=0; i<eWS_Max; i++)
	{
		m_pWeatherBtn[i] = (cButton*)GetWindowForID(WEATHER_BTN_CLEAR + i);
		m_pWeatherBtn[i]->SetActive(FALSE);
		m_pWeatherBtn[i]->SetDisable(TRUE);
	}
}

void CWeatherDialog::SetWeather(WORD wState)
{
	int i;
	for(i=0; i<eWS_Max; i++)
	{
		m_pWeatherBtn[i]->SetDisable(FALSE);

		if(i == (int)wState)
			m_pWeatherBtn[i]->SetActive(TRUE);
		else
			m_pWeatherBtn[i]->SetActive(FALSE);

		m_pWeatherBtn[i]->SetDisable(TRUE);
	}
}
