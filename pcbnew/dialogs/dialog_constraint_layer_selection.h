#ifndef __dialog_constraint_layer_selection__
#define __dialog_constraint_layer_selection__

/**
@file
Subclass of DIALOG_CONSTRAINT_LAYER_SELECTION_BASE, which is generated by wxFormBuilder.
*/

#include <../class_board.h>

#include "dialog_design_rules.h"
#include "dialog_constraint_layer_selection_base.h"

//// end generated include

/** Implementing DIALOG_CONSTRAINT_LAYER_SELECTION_BASE */
class DIALOG_CONSTRAINT_LAYER_SELECTION : public DIALOG_CONSTRAINT_LAYER_SELECTION_BASE
{
private:
    unsigned int m_layerSelection;
    
    PCB_EDIT_FRAME* m_Parent;
	public:
		/** Constructor */
		DIALOG_CONSTRAINT_LAYER_SELECTION( PCB_EDIT_FRAME* parent , unsigned int aLayers );
                ~DIALOG_CONSTRAINT_LAYER_SELECTION( ) { }
                
                virtual unsigned int GetConstraintLayers();
	//// end generated class members
	
};

#endif // __dialog_constraint_layer_selection__