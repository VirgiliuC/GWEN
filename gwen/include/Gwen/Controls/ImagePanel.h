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

				GWEN_CONTROL( ImagePanel, Controls::Base );

				virtual ~ImagePanel();
				virtual void SetUV( float u1, float v1, float u2, float v2 );

				virtual void LocalToUV( const Gwen::Point &p, float &u, float &v);
				virtual Gwen::Point UVToLocal(float u, float v);

				virtual void SetImage( const TextObject & imageName )
				{
					m_Texture.Load( imageName, GetSkin()->GetRender() );
					SetTexImage(m_Texture);
				}
                virtual void SetTexImage(const Texture & texobj,float u1=0, float v1=0, float u2=1, float v2=1);

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

				virtual void SizeToContents();

				virtual void FitToParent();

				virtual bool SetSize(int sw, int sh);

				float   GetScaleX() const {return m_Scale[0];}
				float   GetScaleY() const {return m_Scale[1];}


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

				virtual void Layout( Skin::Base* skin );

				virtual void Render( Skin::Base* skin );
			protected:

				Texture			m_Texture;
				float			m_uv[4];
				float           m_Scale[2];
				Gwen::Color		m_DrawColor;

				bool			m_bStretch;

		};
	}
}
#endif
