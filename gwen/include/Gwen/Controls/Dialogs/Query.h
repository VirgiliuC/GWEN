#pragma once
#ifndef GWEN_CONTROLS_DIALOGS_QUERY_H
#define GWEN_CONTROLS_DIALOGS_QUERY_H

#include "Gwen/Gwen.h"

namespace Gwen
{
	namespace Dialogs
	{
		//TODO. YesNo, Ok etc

        enum EDlgType {DT_OK_CANCEL, DT_QUERY, DT_QUERY_CANCEL};
        enum EDlgResult {DR_OK, DR_CANCEL, DR_YES, DR_NO};

//        class MessageBox :  public Gwen::Controls::WindowCanvas{
//
//        public:
//        public:
//
//             MessageBox( int x, int y, int w, int h, &skin,
//                        const TextObject& title,
//                        const TextObject& text,
//                        EDlgFlags flags);
//
//            void        SetTitle(const TextObject& title);
//            void        SetText(const TextObject& title);
//            void        SetIcon(const Texture&)
//            EDlgType    GetType() const {return m_Flag;}
//
//            EDlgResult  Show()
//            {
//                while (!WantsQuit() ){
//                    DoThink();
//                }
//                return EDlgResult;
//            };
//
//        private:
//
//            void MakeGUI();
//
//        private:
//
//            Texture     m_Icon;
//            EDlgResult  m_Result;
//            EDlgType    m_Type;
//        };
	}
}
#endif
