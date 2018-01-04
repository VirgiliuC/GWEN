/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Dialogs/FileOpen.h"

using namespace Gwen;
using namespace Gwen::Dialogs;

void Gwen::Dialogs::FileOpenEx( bool bUseSystem,
                               const String & Name, const String & StartPath, const String & Extension,
                               List & sqFileName,bool MultiSelect,
                               Gwen::Event::Handler* pHandler, Gwen::Event::Handler::FunctionWithInformation fnCallback )
{
	if ( bUseSystem && Gwen::Platform::FileOpen( Name, StartPath, Extension, sqFileName, MultiSelect, pHandler, fnCallback ) )
	{
		return;
	}

	//
	// TODO: SHOW GWEN FILE SELECTION DIALOG
	//
}
