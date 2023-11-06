
//#define ZOOMRATE -80.0f
//#define ZOOMRATE -5.0f
#define FLYRATE -1.0f
extern float ZOOMRATE;
//////////////////////////////////////////////////////////////////////////
// Ä«¸Þ¶ó ÁÜÀÎ¾Æ¿ô
void CameraWheelZoom(CKeyboard* Keyboard,CMouse* Mouse)
{
	cWindow* pWnd = NULL ;

	PTRLISTPOS pos = WINDOWMGR->m_pWindowList->GetHeadPosition() ;

	BOOL bResult = FALSE ;

	while(pWnd = (cWindow*)WINDOWMGR->m_pWindowList->GetNext(pos))
	{
		if( pWnd )
		{
			if( pWnd->IsActive() )
			{
				LONG xPos = MOUSE->GetMouseX() ;
				LONG yPos = MOUSE->GetMouseY() ;

				if(pWnd->PtInWindow(xPos, yPos))
				{
					bResult = TRUE ;
				}
			}
		}
	}

	if( !bResult )
	{
		int Wheel = Mouse->GetWheel();
		if(Wheel)
		{
			if( CAMERA->GetCameraMode() == eCM_EyeView )
			{
				float fWheel = Wheel * FLYRATE;
				CAMERA->Fly(fWheel);
			}
			else
			{
				float fWheel = Wheel * ZOOMRATE;
				CAMERA->ZoomDistance(0,fWheel,0);
			}
		}
	}
}
