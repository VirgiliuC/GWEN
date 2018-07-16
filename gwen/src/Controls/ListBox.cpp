/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/


#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/InputHandler.h"

using namespace Gwen;
using namespace Gwen::Controls;

class ListBoxRow : public Layout::TableRow
{
    public:
		GWEN_CONTROL_INLINE( ListBoxRow, Layout::TableRow )
		{
			SetMouseInputEnabled( true );
			SetSelected( false );
		}

		void Render( Skin::Base* skin )
		{
			skin->DrawListBoxLine( this, IsSelected(), GetEven() );
		}

		bool IsSelected() const
		{
			return m_bSelected;
		}

		void DoSelect()
		{
			SetSelected( true );
			onRowSelected.Call( this );
			Redraw();
		}

		void OnMouseClickLeft( int /*x*/, int /*y*/, bool bDown )
		{
			if ( bDown )
			{
				DoSelect();
			}
		}

		void SetSelected( bool b )
		{
			if(not m_bSelected)
                saveTextColors();

			m_bSelected = b;
			// TODO: Get this value from the skin.
			if ( b )
			{ SetTextColor(Gwen::Colors::White /*GetSkin()->Colors.Text*/ ); }
			else
			{ restoreTextColors(); }
		}

    private:

        void saveTextColors()
        {
            m_TextColors.resize(GetColumnCount());
            for ( int i = 0; i < GetColumnCount(); i++ ){
                 if ( !GetCellContents(i) ) { continue; }
                 m_TextColors[i] = GetCellContents(i)->TextColor();
            }
        }

        void restoreTextColors()
        {
            for ( int i = 0; i < GetColumnCount(); i++ ){
                 if ( !GetCellContents(i) ) { continue; }
                 GetCellContents(i)->SetTextColor( m_TextColors[i] );
            }
        }

	private:
        std::vector<Gwen::Color> m_TextColors;
		bool			m_bSelected;

};

GWEN_CONTROL_CONSTRUCTOR( ListBox )
{
	SetScroll( false, true );
	SetAutoHideBars( true );
	SetMargin( Margin( 1, 1, 1, 1 ) );
	m_InnerPanel->SetPadding( Padding( 2, 2, 2, 2 ) );
	m_Table = new Controls::Layout::Table( this );
	m_Table->SetColumnCount( 1 );
	m_bMultiSelect = false;
}

Layout::TableRow* ListBox::AddItem( const TextObject & strLabel, const String & strName )
{
	ListBoxRow* pRow = new ListBoxRow( this );
	m_Table->AddRow( pRow );
	pRow->SetCellText( 0, strLabel );
	pRow->SetName( strName );
	pRow->onRowSelected.Add( this, &ListBox::OnRowSelected );
	return pRow;
}

void ListBox::UnselectItem( Layout::TableRow* row )
{
	ListBox::Rows::iterator it = std::find( m_SelectedRows.begin(), m_SelectedRows.end(), row );
	if( it != m_SelectedRows.end() ) {
		m_SelectedRows.erase( it );
	}
}
void ListBox::RemoveItem( Layout::TableRow* row )
{
	UnselectItem(row);
	m_Table->Remove( row );
}

void ListBox::Render( Skin::Base* skin )
{
	skin->DrawListBox( this );
}

void ListBox::Layout( Skin::Base* skin )
{
	BaseClass::Layout( skin );
	const Gwen::Rect & inner = m_InnerPanel->GetInnerBounds();
	m_Table->SetPos( inner.x, inner.y );
	m_Table->SetWidth( inner.w );
	m_Table->SizeToChildren( false, true );
	BaseClass::Layout( skin );
}

void ListBox::UnselectAll()
{
	std::list<Layout::TableRow*>::iterator it = m_SelectedRows.begin();

	while ( it != m_SelectedRows.end() )
	{
		ListBoxRow* pRow = static_cast<ListBoxRow*>( *it );
		it = m_SelectedRows.erase( it );
		pRow->SetSelected( false );
	}
}

Layout::TableRow* ListBox::GetSelectedRow()
{
	if ( m_SelectedRows.empty() ) { return NULL; }
    //return the last selected row
	return m_SelectedRows.back();
}

ListBox::RowIndices
ListBox::GetSelectedIndices() const
{
	RowIndices index_list;
	if ( not m_SelectedRows.empty() ) {
        auto selected_rows = GetSelectedRows ();
        for(auto row : selected_rows){
            int index = m_Table->GetChildIndex(row);
            index_list.push_back(index);
        }
	}
	return index_list;
}


Gwen::String ListBox::GetSelectedRowName()
{
	Layout::TableRow* row = GetSelectedRow();

	if ( !row ) { return ""; }

	return row->GetName();
}

void ListBox::Clear()
{
	UnselectAll();
	m_Table->Clear();
}

void ListBox::OnRowSelected( Base* pControl )
{
	if ( not AllowMultiSelect() ) {
        SetSelectedRow( pControl, true);
        return;
    }
	if (Gwen::Input::IsControlDown()){
        ListBoxRow* pRow = gwen_cast<ListBoxRow> ( pControl );
        auto it = std::find(m_SelectedRows.begin(), m_SelectedRows.end(), pRow);
        if(it != m_SelectedRows.end()){
            pRow->SetSelected( false );
            m_SelectedRows.erase(it);
        }
        else
            SetSelectedRow( pRow, false);//don't clear the others
        return;
    }
	if (Gwen::Input::IsShiftDown()){
     //on 'Shift', select all the rows between the clicked and last-selected ones
        int first = GetSelectedRowIndex();
        if(first < 0) {
            SetSelectedRow( pControl, false);
            return;
        }
        int last =  m_Table->GetChildIndex(pControl);
        if(last < 0) return;
        auto from = std::min(first, last);
        auto to = std::max(first, last);
        for(; from <= to; from++){
            pControl = m_Table->GetChild(from);
            SetSelectedRow( pControl, false);
        }
        return;
	}
	SetSelectedRow( pControl, true);
}

void ListBox::SetSelectedRow( Gwen::Controls::Base* pControl, bool bClearOthers )
{
	if ( bClearOthers )
	{ UnselectAll(); }

	ListBoxRow* pRow = gwen_cast<ListBoxRow> ( pControl );

	if ( !pRow ) { return; }
    bool found = false;
	//make sure this is one of our rows!
	if(not GetTable()->IsChild(pRow)) { return; }//not ours

	for(auto sel_row : m_SelectedRows){
        if(sel_row == pRow)
            return;//selected already
	}
	//if we're here, then the row is ours and not yet marked as selected
	m_SelectedRows.push_back( pRow );
	pRow->SetSelected( true );
	onRowSelected.Call( this );
}



void ListBox::SelectByString( const TextObject & strName, int cell, bool bClearOthers )
{
	if ( bClearOthers )
	{ UnselectAll(); }

	Base::List & children = m_Table->GetChildren();

	for ( Base::List::iterator iter = children.begin(); iter != children.end(); ++iter )
	{
		ListBoxRow* pChild = gwen_cast<ListBoxRow> ( *iter );

		if ( !pChild ) { continue; }

		if ( Utility::Strings::Wildcard( strName, pChild->GetText( cell ) ) )
		{ SetSelectedRow( pChild, false ); }
	}
}

bool ListBox::OnKeyDown( bool bDown )
{
	if ( bDown )
	{
		Base::List & children = m_Table->Children;
		Base::List::const_iterator begin = children.begin();
		Base::List::const_iterator end = children.end();
		Controls::Base* sel_row = GetSelectedRow();

		if ( sel_row == NULL && !children.empty() ) // no user selection yet, so select first element
		{ sel_row = children.front(); }

		Base::List::const_iterator result = std::find( begin, end, sel_row );

		if ( result != end )
		{
			Base::List::const_iterator next = result;
			++next;

			if ( next != end )
			{ result = next; }

			ListBoxRow* pRow = gwen_cast<ListBoxRow> ( *result );

			if ( pRow )
			{
				pRow->DoSelect();
				Controls::VerticalScrollBar* pScroll = gwen_cast<Controls::VerticalScrollBar> ( m_VerticalScrollBar );

				if ( pScroll ) { pScroll->NudgeDown( this ); }

				Redraw();
			}
		}
	}

	return true;
}

bool ListBox::OnKeyUp( bool bDown )
{
	if ( bDown )
	{
		Base::List & children = m_Table->Children;
		Base::List::const_iterator begin = children.begin();
		Base::List::const_iterator end = children.end();
		Controls::Base* sel_row = GetSelectedRow();

		if ( sel_row == NULL && !children.empty() ) // no user selection yet, so select first element
		{ sel_row = children.front(); }

		Base::List::const_iterator result = std::find( begin, end, sel_row );

		if ( result != end )
		{
			if ( result != begin )
			{ --result; }

			ListBoxRow* pRow = gwen_cast<ListBoxRow> ( *result );

			if ( pRow )
			{
				pRow->DoSelect();
				Controls::VerticalScrollBar* pScroll = gwen_cast<Controls::VerticalScrollBar> ( m_VerticalScrollBar );

				if ( pScroll ) { pScroll->NudgeUp( this ); }

				Redraw();
			}
		}
	}

	return true;
}
