/*****************************************************************************
 *  Module for Microchip Graphics Library 
 *  Grid control
 *****************************************************************************
 * FileName:        Grid.c
 * Dependencies:    string.h, Graphics.h 
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright � 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Kim Otten            02/29/08    ...
 *****************************************************************************/

/*
    Grid Object

TODO: Currently the grid does not support scroll bars. It must fit on the screen.

*/

#include <string.h>
#include "Graphics\Graphics.h"

//#include "uart2.h"

#define CELL_AT(c,r)    ((c * pGrid->numRows) + r)

GRID *GridCreate( WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, SHORT numColumns, SHORT numRows,
                    SHORT cellWidth, SHORT cellHeight, GOL_SCHEME *pScheme )
{
    GRID *pGrid = NULL;

    if ((pGrid = malloc(sizeof(GRID))) == NULL)
    {
        return NULL;
    }

    if ((pGrid->gridObjects = malloc(sizeof(GRIDITEM)*numColumns*numRows)) == NULL)
    {
        free( pGrid );
        return NULL;
    }

    // Initialize grid items to default.
    memset( pGrid->gridObjects, 0, sizeof(GRIDITEM)*numColumns*numRows );

    // Initialize grid
    pGrid->ID           = ID;
    pGrid->pNxtObj      = NULL;
    pGrid->type         = OBJ_GRID;
    pGrid->state        = state;
    pGrid->left         = left;
    pGrid->top          = top;
    pGrid->right        = right;
    pGrid->bottom       = bottom;
    pGrid->numColumns   = numColumns;
    pGrid->numRows      = numRows;
    pGrid->cellWidth    = cellWidth;
    pGrid->cellHeight   = cellHeight;
    pGrid->focusX       = 0;
    pGrid->focusY       = 0;

    // Set the color scheme to be used
    if (pScheme == NULL)
    {
        pGrid->pGolScheme = _pDefaultGolScheme;
    }
    else
    {
        pGrid->pGolScheme = (GOL_SCHEME *)pScheme;
    }

    GOLAddObject((OBJ_HEADER*) pGrid);

    return pGrid;
}



#define CELL_LEFT               (1 + pGrid->left + (i * (pGrid->cellWidth  + 1)))
#define CELL_TOP                (1 + pGrid->top +  (j * (pGrid->cellHeight + 1)))
#define CELL_RIGHT              (CELL_LEFT + pGrid->cellWidth  - 1)
#define CELL_BOTTOM             (CELL_TOP  + pGrid->cellHeight - 1)
#define BITMAP_SCALE            1

WORD GridDraw( GRID *pGrid )
{
    SHORT   i;
    SHORT   j;

    if ((pGrid->state & GRID_DRAW_ITEMS) || (pGrid->state & GRID_DRAW_ALL) || (pGrid->state & GRID_SHOW_FOCUS))
    {
        if (pGrid->state & GRID_DRAW_ALL)
        {
            // Clear the entire region.
            SetColor( pGrid->pGolScheme->CommonBkColor );
            Bar( pGrid->left, pGrid->top, pGrid->right, pGrid->bottom );

            // Draw the grid lines
            if (pGrid->state & (GRID_SHOW_LINES | GRID_SHOW_BORDER_ONLY | GRID_SHOW_SEPARATORS_ONLY))
            {
                SetLineType( SOLID_LINE );
                SetColor( pGrid->pGolScheme->EmbossLtColor  );

                // Draw the outside of the box
                // TODO This should have some 3D effects added with GOL_EMBOSS_SIZE
                if (pGrid->state & (GRID_SHOW_LINES | GRID_SHOW_BORDER_ONLY))
                {
                    Line( pGrid->left, pGrid->top,  pGrid->right, pGrid->top );
                    LineTo( _cursorX, pGrid->bottom );
                    LineTo( pGrid->left, _cursorY );
                    LineTo( pGrid->left, pGrid->top );
                }    

                // Draw the lines between each cell
                if (pGrid->state & (GRID_SHOW_LINES | GRID_SHOW_SEPARATORS_ONLY))
                {
                    for (i=1; i< pGrid->numColumns; i++)
                    {
                        Line( pGrid->left + i * (pGrid->cellWidth+1), pGrid->top, 
                              pGrid->left + i * (pGrid->cellWidth+1), pGrid->top + pGrid->numRows * (pGrid->cellHeight+1) );
                    }
                    for (i=1; i<pGrid->numRows; i++)
                    {
                        Line( pGrid->left,  pGrid->top + i * (pGrid->cellHeight+1),  
                              pGrid->right, pGrid->top + i * (pGrid->cellHeight+1) );
                    }
                }    
            }
        }

        for (i = 0; i < pGrid->numColumns; i++)
        {
            for (j = 0; j < pGrid->numRows; j++)
            {
                if ((pGrid->state & GRID_DRAW_ALL) ||
                    ((pGrid->state & GRID_DRAW_ITEMS) && (pGrid->gridObjects[CELL_AT(i,j)].status & GRIDITEM_DRAW)) || 
                    ((pGrid->state & GRID_SHOW_FOCUS) && (i == pGrid->focusX) && (j == pGrid->focusY)))
                {
                    // Clear the cell
                    SetColor( pGrid->pGolScheme->CommonBkColor );
                    Bar( CELL_LEFT, CELL_TOP, CELL_RIGHT, CELL_BOTTOM );

                    // Draw the cell
                    if ((pGrid->gridObjects[CELL_AT(i,j)].status & GRID_TYPE_MASK) == GRIDITEM_IS_BITMAP)
                    {
                        // Draw the bitmap
                        if (pGrid->gridObjects[CELL_AT(i,j)].data)
                        {
                            PutImage( CELL_LEFT, CELL_TOP, pGrid->gridObjects[CELL_AT(i,j)].data, BITMAP_SCALE );
                        }
                    }
                    else
                    { 
                        // Draw the text
                    }
    
        	        // Draw the focus if applicable.
        	        if ((pGrid->state & GRID_SHOW_FOCUS) && (i == pGrid->focusX) && (j == pGrid->focusY))
        	        {
                        SetColor( pGrid->pGolScheme->EmbossLtColor );
                        SetLineType( DOTTED_LINE );
                        SetLineThickness( NORMAL_LINE );   	        
                        Rectangle( CELL_LEFT, CELL_TOP, CELL_RIGHT, CELL_BOTTOM );
                    }  
                    
                    // If the cell is selected, indicate it.
                    if (pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status & GRIDITEM_SELECTED)
                    {
                        SetColor( pGrid->pGolScheme->EmbossLtColor );
                        SetLineType( SOLID_LINE ); 
                        if (pGrid->state & GRID_SHOW_LINES)
                        {
                            SetLineThickness( THICK_LINE );   	        
                        }
                        else
                        {
                            SetLineThickness( NORMAL_LINE );   	        
                        }    
                        Rectangle( CELL_LEFT-1, CELL_TOP-1, CELL_RIGHT+1, CELL_BOTTOM+1 );
                    }
                    
                    GridClearCellState( pGrid, i, j, GRIDITEM_DRAW );  
                }
            }
        }
        
        //pGrid->state &= ~(GRID_DRAW_ITEMS || GRID_DRAW_ALL || GRID_SHOW_FOCUS); 
        pGrid->state &= ~(GRID_DRAW_ITEMS || GRID_DRAW_ALL ); 
        
        // Set line state
        SetLineType( SOLID_LINE );
    }

    return 1;
}


void GridFreeItems( GRID *pGrid )
{
    if (pGrid && pGrid->gridObjects)
    {
        free( pGrid->gridObjects );
        pGrid->gridObjects = NULL;  // Just in case...
    }    
}    


WORD GridSetCell( GRID *pGrid, SHORT column, SHORT row, WORD state, void *data )
{
    if ((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return GRID_OUT_OF_BOUNDS;
    }

    pGrid->gridObjects[CELL_AT(column,row)].data    = data;
    pGrid->gridObjects[CELL_AT(column,row)].status  = state; // This overwrites GRIDITEM_SELECTED

    return GRID_SUCCESS;
}

void GridClearCellState( GRID *pGrid, SHORT column, SHORT row, WORD state )
{
    if ((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }

    pGrid->gridObjects[CELL_AT(column,row)].status  &= ~state;

    return;
}

void GridSetFocus( GRID *pGrid, SHORT column, SHORT row )
{
    if ((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }
    
    pGrid->focusX = column;
    pGrid->focusY = row;
}
    
void GridSetCellState( GRID *pGrid, SHORT column, SHORT row, WORD state )
{
    if ((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return;
    }

    pGrid->gridObjects[CELL_AT(column,row)].status  |= state;

    return;
}

void * GridGetCell( GRID *pGrid, SHORT column, SHORT row, WORD *cellType )
{
    if ((column >= pGrid->numColumns) || (row >= pGrid->numRows))
    {
        return NULL;
    }

    *cellType = pGrid->gridObjects[CELL_AT(column,row)].status & GRID_TYPE_MASK;

    return pGrid->gridObjects[CELL_AT(column,row)].data;
}


//SHORT GridGetSelectedColumn(

//SHORT GridGetSelectedRow()


void GridMsgDefault( WORD translatedMsg, GRID *pGrid, GOL_MSG *pMsg )
{
    switch (translatedMsg)
    {
        case GRID_MSG_ITEM_SELECTED:
            // Currently, only a single item can be selected.  This can be expanded later,
            // when touchscreen support is enhanced.
            pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status ^= GRIDITEM_SELECTED;
//            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState( pGrid, GRID_DRAW_ITEMS );
            break;

	    case GRID_MSG_UP:
	        if (pGrid->focusY > 0)
	        {
                pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status |= GRIDITEM_DRAW;
    	        pGrid->focusY --;
//            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState( pGrid, GRID_DRAW_ITEMS );
    	    } 
	        break;

	    case GRID_MSG_DOWN:
	        if (pGrid->focusY < (pGrid->numRows-1))
	        {
                pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status |= GRIDITEM_DRAW;
    	        pGrid->focusY ++;
//            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState( pGrid, GRID_DRAW_ITEMS );
    	    } 
	        break;

	    case GRID_MSG_LEFT:
	        if (pGrid->focusX > 0)
	        {
                pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status |= GRIDITEM_DRAW;
    	        pGrid->focusX --;
//            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState( pGrid, GRID_DRAW_ITEMS );
    	    } 
	        break;

	    case GRID_MSG_RIGHT:
	        if (pGrid->focusX < (pGrid->numColumns-1))
	        {
                pGrid->gridObjects[CELL_AT(pGrid->focusX,pGrid->focusY)].status |= GRIDITEM_DRAW;
    	        pGrid->focusX ++;
//            SetState( pGrid, GRID_SHOW_FOCUS | GRID_DRAW_ITEMS );
            SetState( pGrid, GRID_DRAW_ITEMS );
    	    } 
            break;
    }
}


WORD GridTranslateMsg(GRID *pGrid, GOL_MSG *pMsg)
{
    // Evaluate if the message is for the check box
    // Check if disabled first
    if ( GetState( pGrid, GRID_DISABLED ) )
    {
        return OBJ_MSG_INVALID;
    }

    #ifdef USE_TOUCHSCREEN
        if(pMsg->type == TYPE_TOUCHSCREEN)
        {
            // Check if it falls in the check box borders
            if( (pGrid->left   <= pMsg->param1) &&
                (pGrid->right  >= pMsg->param1) &&
                (pGrid->top    <= pMsg->param2) &&
                (pGrid->bottom >= pMsg->param2) )
            {
                return GRID_MSG_TOUCHED;
            }

            return OBJ_MSG_INVALID;
        }
    #endif

    #ifdef USE_KEYBOARD
        if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
            (pMsg->type == TYPE_KEYBOARD) &&
            (pMsg->param1 == pGrid->ID))
        {
            if ((pMsg->param2 == SCAN_SPACE_PRESSED) ||
                (pMsg->param2 == SCAN_CR_PRESSED))
            {
                return GRID_MSG_ITEM_SELECTED;
            }
            else if (pMsg->param2 == SCAN_LEFT_PRESSED) 
            {
                return GRID_MSG_LEFT;
            }
            else if (pMsg->param2 == SCAN_RIGHT_PRESSED)
            {
                return GRID_MSG_RIGHT;
            }
            else if (pMsg->param2 == SCAN_UP_PRESSED) 
            {
                return GRID_MSG_UP;
            }
            else if (pMsg->param2 == SCAN_DOWN_PRESSED)
            {
                return GRID_MSG_DOWN;
            }
}
        return OBJ_MSG_INVALID;

    #endif

    return OBJ_MSG_INVALID;
}

