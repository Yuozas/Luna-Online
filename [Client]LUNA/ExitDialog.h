#ifndef _EXITDIALOG_H
#define _EXITDIALOG_H

#include "./Interface/cDialog.h"

class CExitDialog : public cDialog
{
protected:
	
public:
	CExitDialog() ;
	~CExitDialog() ;

	virtual void SetActive( BOOL val );
};



#endif