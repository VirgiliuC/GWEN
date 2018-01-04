/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/


#include "Gwen/Controls/ImagePanel.h"


using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( ImagePanel )
{
    SetUV( 0, 0, 1, 1 );
    SetMouseInputEnabled( false );
    m_DrawColor = Colors::White;
    SetStretch( true );
}

ImagePanel::~ImagePanel()
{
    m_Texture.Release( GetSkin()->GetRender() );
}

//***********************************************************************
void ImagePanel::SetUV( float u1, float v1, float u2, float v2 )
{
    m_uv[0] = u1;
    m_uv[1] = v1;
    m_uv[2] = u2;
    m_uv[3] = v2;
}

//***********************************************************************
void ImagePanel::LocalToUV( const Gwen::Point &p, float &u, float &v)
{
    u = m_uv[0]+(float)p.x/m_Scale[0]*(m_uv[2]-m_uv[0])/(float)ImageWidth();
    v = m_uv[1]+(float)p.y/m_Scale[1]*(m_uv[3]-m_uv[1])/(float)ImageHeight();
}
//***********************************************************************
Gwen::Point ImagePanel:: UVToLocal(float u, float v)
{
    Gwen::Point p;
    p.x = (int)(m_Scale[0]*(u-m_uv[0])/(m_uv[2]-m_uv[0])*(float)ImageWidth()+0.5);
    p.y = (int)(m_Scale[1]*(v-m_uv[1])/(m_uv[3]-m_uv[1])*(float)ImageHeight()+0.5);
    return p;
}

//***********************************************************************
void ImagePanel::SetTexImage(const Texture & texobj,float u1, float v1, float u2, float v2)
{
    m_Texture=texobj;
    SetUV( u1, v1, u2, v2 );

    Gwen::Rect rbounds = GetRenderBounds();
    m_Scale[0]=rbounds.w/(float)ImageWidth();
    m_Scale[1]=rbounds.h/(float)ImageHeight();
    if(not m_bStretch)
       m_Scale[0]= m_Scale[1]=std::min(m_Scale[0],m_Scale[1]);
};

//***********************************************************************
void ImagePanel::SizeToContents()
{
    Dock(Pos::None);
    SetSize( ImageWidth(), ImageHeight());
}

//***********************************************************************
void ImagePanel::FitToParent()
{
    if(GetParent())
        Dock(Pos::Fill);
//        SetBounds(GetParent()->GetRenderBounds());
}

//***********************************************************************
bool ImagePanel::SetSize(int sw, int sh)
{
    bool res=Controls::Base::SetSize( sw, sh);
    Gwen::Rect rbounds = GetRenderBounds();
    m_Scale[0]=rbounds.w/(float)ImageWidth();
    m_Scale[1]=rbounds.h/(float)ImageHeight();
    if(not m_bStretch)
       m_Scale[0]= m_Scale[1]=std::min(m_Scale[0],m_Scale[1]);
    return res;
}

//***********************************************************************
void ImagePanel::Layout( Skin::Base* skin )
{
    Controls::Base::Layout(skin);
    Gwen::Rect rbounds = GetRenderBounds();
    m_Scale[0]=rbounds.w/(float)ImageWidth();
    m_Scale[1]=rbounds.h/(float)ImageHeight();
    if(not m_bStretch)
       m_Scale[0]= m_Scale[1]=std::min(m_Scale[0],m_Scale[1]);
}

//***********************************************************************
void ImagePanel::Render( Skin::Base* skin )
{
    skin->GetRender()->SetDrawColor( m_DrawColor );

    if ( m_bStretch ){
        skin->GetRender()->DrawTexturedRect(&m_Texture, GetRenderBounds(),
                                            m_uv[0], m_uv[1], m_uv[2], m_uv[3] );
    }
    else{
        skin->GetRender()->DrawTexturedRect(&m_Texture, Gwen::Rect( 0, 0,m_Scale[0]*ImageWidth(), m_Scale[1]*ImageHeight()),
                                            m_uv[0], m_uv[1], m_uv[2], m_uv[3] );
    }
}
