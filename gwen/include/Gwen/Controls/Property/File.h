
#pragma once
#ifndef GWEN_CONTROLS_PROPERTY_FILE_H
#define GWEN_CONTROLS_PROPERTY_FILE_H

#include "Gwen/Controls/Properties.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/Dialogs/FolderOpen.h"

namespace Gwen
{
	namespace Controls
	{
		namespace Property
		{
			class File : public Property::Text
			{
                public:
					GWEN_CONTROL_INLINE( File, Property::Text )
					{
						Controls::Button* pButton = new Controls::Button( this );
						pButton->Dock( Pos::Right );
						pButton->SetText( "..." );
						pButton->SetWidth( 20 );
						pButton->onPress.Add( this, &ThisClass::OnButtonPress );
						pButton->SetMargin( Margin( 1, 1, 1, 2 ) );
						m_strDialogName = "Find File";
						m_strFileExtension = "*.*";
						SetMultiSelection(false);
					}

					File* SetExtension( const Gwen::String & string )
					{
						m_strFileExtension = string;
						return this;
					}

                    void SetMultiSelection(bool multi_sel)
                    {
                        m_MultiSelection = multi_sel;
                    }


                private:

					void OnButtonPress( Controls::Base* control )
					{
						Gwen::List sqFileName;
						Gwen::Dialogs::FileOpen( true, m_strDialogName, m_TextBox->GetText().Get(),
                                                m_strFileExtension, sqFileName,false /*m_MultiSelection*/, nullptr, nullptr);
                        if(not sqFileName.empty())
                            m_TextBox->SetText( sqFileName[0] );
					}
                private:

					String	m_strDialogName;
					String	m_strFileExtension;
					bool    m_MultiSelection;
			};
		}
	}
}
#endif
