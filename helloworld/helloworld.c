/*===========================================================================

FILE: Helloworld.c
  
SERVICES: Sample applet using AEE
    
DESCRIPTION
   This file contains a very simple sample application that displays the
   "Hello World" on the display.
      
PUBLIC CLASSES:  
   N/A
        
INITIALIZATION AND SEQUENCING REQUIREMENTS:
   The following explanation applies to this sample containing one applet which serves
   as a base for app developers to create their own applets using AEE Services:
   

   In the applet source file (like this one), include AEEAppGen.h.

   Mandatory Sections in Applet Source (this file):
   -----------------------------------------------
   Following Mandatory Sections are required for each applet source file. 
   (Search for "Mandatory" to identify these sections)

   Includes:     
      Copy this section as-is from the sample applet. It contains:
      AEEAppGen.h: For AEEApplet declaration

   Type Declarations:
		A data structure is usually defined to hold the app specific data. In this structure,
		the first element must be of type AEEApplet.  Note that this simple example does 
      not require any additional data, so this step has been removed.

   Functions: (For more details, see corresponding function description in this applet)

      App_HandleEvent(): This function is the Event Handler for the applet.
                         Copy the function outline from the sample applet and add app
                         specific code.

      AEEClsCreateInstance(): This function is called to create an instance of the applet.
                              It is called by AEEModGen when applet is being created.

   
   Important Notes:
   ---------------
   1. DO NOT use any "static data" in the applet. Always use the functions exported by
	   AEEStdlib or by IHeap services to dynamically allocate data and make it a member of 
		the applet structure. 
   2. DO NOT include and link "standard C library". Use AEE Memory Services (in AEEHeap.h) and Standard Library macros(in AEEStdLib.h).
      For example, use MALLOC() to allocate memory, WSTRCPY() to make a copy of Unicode (wide) string.
   3. BREW is Unicode(wide string) compliant ONLY (no ISOLATIN1/ANSI) except for file names which are ISOLATIN1/ANSI.
      ALWAYS USE AECHAR instead of "char". Use string services provided in AEEStdLib.h for string manipulation.
   4. It is always a good idea to DEFINE RESOURCES using BREW ResourceEditor. Make Strings, Bitmaps, Dialogs, etc. 
      as resources. ResourceEditor saves resources as .bri file, generates resource header file 
      and compiles .bri into a .bar binary file, which can be used by the applet.

   Miscellanoeus Notes:
   -------------------
   1. Make sure that the class ID used for the app is the same as that defined corresponding in .MIF file
   2. Always make sure that compiled resource (.bar) file and corresponding
      resource header (a) reside in app directory and (b) are included in the applet code.
      Define a constant APP_RES_FILE containing the name of the compiled resource file (with .bar extension).

   More than one applet:
   --------------------
   If more than one applet needs to be defined, then do the following
   (1) Follow the above description for each applet




   	   Copyright © 2000-2002 QUALCOMM Incorporated.
	                  All Rights Reserved.
                   QUALCOMM Proprietary/GTDR
===========================================================================*/


/*===============================================================================
INCLUDES AND VARIABLE DEFINITIONS
=============================================================================== */
#include "AEEAppGen.h"        // Applet helper file
#include "helloworld.bid"		// Applet-specific header that contains class ID
#include "nmdef.h"


/*-------------------------------------------------------------------
Static function prototypes
-------------------------------------------------------------------*/
static boolean HelloWorld_HandleEvent(AEEApplet * pme, AEEEvent eCode,uint16 wParam, uint32 dwParam);

/*===============================================================================
FUNCTION DEFINITIONS
=============================================================================== */
/*===========================================================================

FUNCTION: AEEClsCreateInstance

DESCRIPTION
	This function is invoked while the app is being loaded. All Modules must provide this 
	function. Ensure to retain the same name and parameters for this function.
	In here, the module must verify the ClassID and then invoke the AEEApplet_New() function
	that has been provided in AEEAppGen.c. 

   After invoking AEEApplet_New(), this function can do app specific initialization. In this
   example, a generic structure is provided so that app developers need not change app specific
   initialization section every time except for a call to InitAppData(). This is done as follows:
   InitAppData() is called to initialize AppletData instance. It is app developers 
   responsibility to fill-in app data initialization code of InitAppData(). App developer
   is also responsible to release memory allocated for data contained in AppletData -- this can be
   done in FreeAppData().

PROTOTYPE:
	int AEEAppCreateInstance(AEECLSID clsID, IShell* pIShell, IModule* pIModule,IApplet** ppApplet)

PARAMETERS:
	clsID: [in]: Specifies the ClassID of the applet which is being loaded

	pIShell: [in]: Contains pointer to the IShell interface. 

	pIModule: pin]: Contains pointer to the IModule interface to the current module to which
	this app belongs

	ppApplet: [out]: On return, *ppApplet must point to a valid AEEApplet structure. Allocation
	of memory for this structure and initializing the base data members is done by AEEApplet_New().

DEPENDENCIES
  none

RETURN VALUE
  SUCCESS: If the app needs to be loaded and if AEEApplet_New() invocation was successful
  EFAILED: If the app does not need to be loaded or if errors occurred in AEEApplet_New().
  If this function returns FALSE, the app will not be loaded.

SIDE EFFECTS
  none
===========================================================================*/
int AEEClsCreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * pMod,void ** ppObj)
{
   *ppObj = NULL;
		
   if(AEEApplet_New( sizeof(AEEApplet),                  // Size of our private class
                     ClsId,                              // Our class ID
                     pIShell,                            // Shell interface
                     pMod,                               // Module instance
                     (IApplet**)ppObj,                   // Return object
                     (AEEHANDLER)HelloWorld_HandleEvent, // Our event handler
                     NULL))                              // No special "cleanup" function
      return(AEE_SUCCESS);

	return (EFAILED);
}

/*===========================================================================

FUNCTION HelloWorld_HandleEvent
  
DESCRIPTION
   This is the EventHandler for this app. All events to this app are handled in this
   function. All APPs must supply an Event Handler.  

   Note - The switch statement in the routine is to demonstrate how event handlers are 
   generally structured.  However, realizing the simplicity of the example, the code could
   have been reduced as follows:

   if(eCode == EVT_APP_START){
      IDISPLAY_DrawText();
      IDISPLAY_Update();
      return(TRUE);
   }
   return(FALSE);

   However, while doing so would have demonstrated how BREW apps can be written in about 8
   lines of code (including the app creation function), it might have confused those who wanted 
   a bit more practical example.

   Also note that the use of "szText" below is provided only for demonstration purposes.  As 
   indicated in the documentation, a more practical approach is to load text resources
   from the applicaton's resource file.

   Finally, note that the ONLY event that an applet must process is EVT_APP_START.  Failure
   to return TRUE from this event will indicate that the app cannot be started and BREW
   will close the applet.
    
PROTOTYPE:
   boolean HelloWorld_HandleEvent(IApplet * pi, AEEEvent eCode, uint16 wParam, uint32 dwParam)
      
PARAMETERS:
   pi: Pointer to the AEEApplet structure. This structure contains information specific
   to this applet. It was initialized during the AppCreateInstance() function.
        
   ecode: Specifies the Event sent to this applet
          
   wParam, dwParam: Event specific data.
            
DEPENDENCIES
   none
              
RETURN VALUE
   TRUE: If the app has processed the event
   FALSE: If the app did not process the event
                
SIDE EFFECTS
   none
===========================================================================*/
static boolean HelloWorld_HandleEvent(AEEApplet * pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{  
   AECHAR szText[] = {'H','e','l','l','o',' ','W','o', 'r', 'l', 'd', '\0'};

   switch (eCode){
      case EVT_APP_START:                        
         IDISPLAY_DrawText(pMe->m_pIDisplay,    // Display instance
                           AEE_FONT_BOLD,       // Use BOLD font
                           szText,              // Text - Normally comes from resource
                           -1,                  // -1 = Use full string length
                           0,                   // Ignored - IDF_ALIGN_CENTER
                           0,                   // Ignored - IDF_ALIGN_MIDDLE
                           NULL,                // No clipping
                           IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE);
         IDISPLAY_Update (pMe->m_pIDisplay);

         
			return(TRUE);

      case EVT_APP_STOP:
         return(TRUE);


      default:
         break;
   }
   return(FALSE);
}


