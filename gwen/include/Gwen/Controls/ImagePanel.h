/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_IMAGEPANEL_H
#define GWEN_CONTROLS_IMAGEPANEL_H

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/BaseRender.h"
#include "Gwen/Texture.h"
#include <cmath>

namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT ImagePanel : public Controls::Base
		{
			public:

				GWEN_CONTROL_INLINE( ImagePanel, Controls::Base )
				{
					SetUV( 0, 0, 1, 1 );
					SetMouseInputEnabled( false );
					m_DrawColor = Colors::White;
					SetStretch( true );
				}

				virtual ~ImagePanel()
				{
					m_Texture.Release( GetSkin()->GetRender() );
				}

				virtual void SetUV( float u1, float v1, float u2, float v2 )
				{
					m_uv[0] = u1;
					m_uv[1] = v1;
					m_uv[2] = u2;
					m_uv[3] = v2;
				}

				virtual void SetImage( const TextObject & imageName )
				{
					m_Texture.Load( imageName, GetSkin()->GetRender() );
				}
                virtual void SetTexImage(const Texture & texobj,float u1, float v1, float u2, float v2)
                                                    {m_Texture=texobj;};

				virtual int ImageWidth()    const   {return m_Texture.width*fabs(m_uv[2]-m_uv[0]);}

				virtual int ImageHeight()   const   {return m_Texture.height*fabs(m_uv[3]-m_uv[1]);}

				virtual const Texture& GetImage() const
				{
					return m_Texture;
				}

				virtual int TextureWidth() const
				{
					return m_Texture.width;
				}

				virtual int TextureHeight() const
				{
					return m_Texture.height;
				}

				virtual const TextObject & GetImageName() const
				{
					return m_Texture.name;
				}

				virtual void SizeToContents()
				{
					Dock(Pos::None);
					SetSize( ImageWidth(), ImageHeight());
				}

				virtual void FitToParent()
				{
					if(GetParent())
//                        SetSize(GetParent()->GetSize());
                        Dock(Pos::Fill);
				}

				virtual void SetDrawColor( Gwen::Color color )
				{
					m_DrawColor = color;
				}

				virtual bool FailedToLoad() const
				{
					return m_Texture.FailedToLoad();
				}

				virtual bool GetStretch() const { return m_bStretch; }
				virtual void SetStretch( bool b ) {m_bStretch = b;}

				virtual void Layout( Skin::Base* skin )
				{
                    Controls::Base::Layout(skin);
                    float w = Width(), h=Height();
                    m_Scale = m_bStretch ? 1.0f : std::min(w/(float)ImageWidth(),h/(float)ImageHeight());
				}

				virtual void Render( Skin::Base* skin )
				{
					skin->GetRender()->SetDrawColor( m_DrawColor );

					if ( m_bStretch ){
                        skin->GetRender()->DrawTexturedRect(&m_Texture, GetRenderBounds(),
                                                            m_uv[0], m_uv[1], m_uv[2], m_uv[3] );
                    }
					else{
                        skin->GetRender()->DrawTexturedRect(&m_Texture, Gwen::Rect( 0, 0,m_Scale*ImageWidth(), m_Scale*ImageHeight()),
                                                            m_uv[0], m_uv[1], m_uv[2], m_uv[3] );
                    }
				}
			protected:

				Texture			m_Texture;
				float			m_uv[4];
				float           m_Scale;
				Gwen::Color		m_DrawColor;

				bool			m_bStretch;

		};
	}
}
#endif
