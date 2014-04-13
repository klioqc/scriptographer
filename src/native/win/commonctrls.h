#ifndef _Included_commonctrl_h
#define _Included_commonctrl_h


/*
 * com.scriptographer.widget.Item
 */
//temp //todo: rename without adm, sync with widget itemtype
#define DIAL "ADM Dial Type" // wrapped		
#define FRAME "ADM Frame Type"  // wrapped
	#define ITEMGROUP "ADM Item Group Type" // wrapped
	#define TABBED_MENU "ADM Tabbed Menu Type" // deprecated
	#define LISTBOX "ADM List Box Type" // wrapped
	#define HIERARCHY_LISTBOX "ADM Hierarchy List Box Type" // wrapped
	#define PICTURE_CHECKBOX "ADM Picture Check Box Button Type" // wrapped
	#define PICTURE_PUSHBUTTON "ADM Picture Push Button Type" // wrapped
	#define PICTURE_RADIOBUTTON "ADM Picture Radio Button Type" // wrapped
	#define PICTURE_STATIC "ADM Picture Static Type" // wrapped
	#define POPUP_CONTROL "ADM Popup Control Type"
	#define POPUP_CONTROLBUTTON "ADM Popup Control Button Type"
	#define POPUP_SPINEDIT_CONTROL "ADM Popup Spin Edit Control Type"
	#define POPUP_LIST "ADM Popup List Type" // wrapped
	#define POPUP_MENU "ADM Popup Menu Type" // wrapped
	#define RESIZE "ADM Resize Type" // wrapped  dialog.getResizeButton ) Button)
	#define SCROLLBAR "ADM Scrollbar Type" // wrapped
	#define SCROLLING_POPUP_LIST "ADM Scrolling Popup List Type" // wrapped
	#define SLIDER "ADM Slider Type" // wrapped
	#define SPINEDIT "ADM Spin Edit Type" // wrapped
	#define SPINEDIT_POPUP "ADM Spin Edit Popup Type" // wrapped
	#define SPINEDIT_SCROLLING_POPUP "ADM Spin Edit Scrolling Popup Type" // wrapped
	#define TEXT_CHECKBOX "ADM Text Check Box Type" // wrapped
	#define TEXT_EDIT "ADM Text Edit Type" // wrapped
	#define TEXT_EDIT_READONLY "ADM Text Edit Read-only Type" // wrapped
	#define TEXT_EDIT_MULTILINE "ADM Text Edit Multi Line Type" // wrapped
	#define TEXT_EDIT_MULTILINE_READONLY "ADM Text Edit Multi Line Read-only Type" // wrapped
	#define TEXT_EDIT_POPUP "ADM Text Edit Popup Type" // wrapped
	#define TEXT_EDIT_SCROLLING_POPUP "ADM Text Edit Scrolling Popup Type" // wrapped
	#define TEXT_EDIT_PASSWORD "ADM Password Text Edit Type" // wrapped
	#define TEXT_PUSHBUTTON "ADM Text Push Button Type" // wrapped
	#define TEXT_RADIOBUTTON "ADM Text Radio Button Type" // wrapped
	#define TEXT_STATIC "ADM Text Static Type" // wrapped
	#define TEXT_STATIC_MULTILINE "ADM Text Static Multi Line Type" // wrapped
	#define PROGRESS_BAR "ADM Progress Bar Type" // wrapped
	#define CHASING_ARROWS "ADM Chasing Arrows Type" // wrapped
	#define USER "ADM User Type"
	#define MULTICOLUMN_LISTVIEW "ADM Multi Column List View Type"
	#define SCROLLING_VIEW "ADM Scrolling View Type"
	#define TABGROUP "ADM Tab Group Type"
	// Fake Types
	#define SPACER "ADM Spacer"

 #define USER_CHANGED "ADM User Changed Notifier"
#define 	INTERMEDIATE_CHANGED "ADM Intermediate Changed Notifier"
#define 	BOUNDS_CHANGED "ADM Bounds Changed Notifier"
#define 	VIEW_BOUNDS_CHANGED "ADM View Bounds Changed Notifier"
#define 	ENTRY_TEXT_CHANGED "ADM Entry Text Changed Notifier"
#define 	CLOSE_HIT "ADM Close Hit Notifier"
#define 	ZOOM_HIT "ADM Zoom Hit Notifier"
#define 	CYCLE "ADM Cycle Notifier"
#define 	COLLAPSE "ADM Collapse Notifier"
#define 	EXPAND "ADM Expand Notifier"
#define 	CONTEXT_MENU_CHANGED "ADM Context Menu Changed Notifier"
#define 	WINDOW_SHOW "ADM Show Window Notifier"
#define 	WINDOW_HIDE "ADM Hide Window Notifier"
#define 	GROUP_SHOW "ADM Show Group Notifier"
#define 	GROUP_HIDE "ADM Hide Group Notifier"
#define 	WINDOW_ACTIVATE "ADM Activate Window Notifier"
#define 	WINDOW_DEACTIVATE "ADM Deactivate Window Notifier"
#define 	NUMBER_OUT_OF_BOUNDS "ADM Number Out Of Bounds Notifier"
#define 	WINDOW_DRAG_MOVED "ADM Window Moved By Drag"
#define 	PRE_CLIPBOARD_CUT "ADM Pre Clipboard Cut Notifier"
#define 	POST_CLIPBOARD_CUT "ADM Post Clipboard Cut Notifier"
#define 	PRE_CLIPBOARD_COPY "ADM Pre Clipboard Copy Notifier"
#define 	POST_CLIPBOARD_COPY "ADM Post Clipboard Copy Notifier"
#define 	PRE_CLIPBOARD_PASTE "ADM Pre Clipboard Paste Notifier"
#define 	POST_CLIPBOARD_PASTE "ADM Post Clipboard Paste Notifier"
#define 	PRE_CLIPBOARD_CLEAR "ADM Pre Clipboard Clear Notifier"
#define 	POST_CLIPBOARD_CLEAR "ADM Post Clipboard Clear Notifier"
#define 	PRE_TEXT_SELECTION_CHANGED "ADM Pre Selection Change Notification"
#define 	TEXT_SELECTION_CHANGED "ADM Text Selection Change Notification"
#define 	PRE_CLIPBOARD_REDO "ADM Pre Clipboard Redo Notifier"
#define 	POST_CLIPBOARD_REDO "ADM Post Clipboard Redo Notifier"
#define 	PRE_CLIPBOARD_UNDO "ADM Pre Clipboard Undo Notifier"
#define 	POST_CLIPBOARD_UNDO "ADM Post Clipboard Undo Notifier"
 




class CommonControl;

typedef int (*CommonControlInitProc)(CommonControl* inItem);
typedef void (*CommonControlDestroyProc)(CommonControl* inItem);
typedef void (*CommonControlNotifyProc)(CommonControl* inItem, char * notifier);



class CommonControl
{

public:
  CommonControl(int id) {
    controlID = id;
  }

public:
  void SetInitProc(CommonControlInitProc initProc);
  void SetNotifyProc(CommonControlNotifyProc notifyProc);
  void SetDestroyProc(CommonControlDestroyProc destroyProc);
  


  
   bool OnCreate()
   {
     if (initProc != NULL)
       initProc(this);
      return true;
   }
   bool OnDestroy()
   {
      if (destroyProc != NULL)
       destroyProc(this);
      return true;
   }


  void * GetUserData();
  void SetUserData(void *);

    virtual bool Create(HWND hDlg, RECT* rect);
    virtual bool OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam);


    void SetWindowSize(int width, int height)
    {
        SetWindowPos(hWnd, 0, 0,0,width,height, SWP_NOMOVE);
    }
    void GetBounds(RECT * rc)
    {
       GetWindowRect(hWnd, rc);
    }
    void SetBounds(int x,int y, int width, int height)
    {
      SetWindowPos(hWnd, NULL, x, y, width, height, SWP_SHOWWINDOW);
    }

    void SetText(char *text)
    {
       SetWindowText(hWnd, text);
    }

private:
    void * userData;


protected:
    int controlID;
    HWND hWnd;

      
    CommonControlInitProc initProc;
    CommonControlNotifyProc notifyProc;
    CommonControlDestroyProc destroyProc;

};


#include <hash_map>
typedef stdext::hash_map<int, CommonControl *> ControlMap;

class CommonControlManager
{
public:
   CommonControlManager();

   CommonControl * CreateItem(HWND hDlg, char * itemType,  RECT* rect, CommonControlInitProc Item_onInit, void * userData);

   bool OnParentNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

   bool OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

   CommonControl * GetControl(int id)
   {
        ControlMap::iterator it = ctrlMap.find(id);
	      if (it != ctrlMap.end()) {
          return it->second;
        }
        return NULL; //todo throw
   }

private:
  static int nextid;
  ControlMap ctrlMap;
};


extern CommonControlManager*  commonCtrlManager;

#endif