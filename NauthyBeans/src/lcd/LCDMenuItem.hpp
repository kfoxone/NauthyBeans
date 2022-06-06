/* ***********************************************************
    Note :

    Serial must be initialize before using ScanI2C
*/

#ifndef LCDMENUITEM_H
#define LCDMENUITEM_H


#include <Arduino.h>
#include <iostream>
#include <list>
#include <iterator>

/// Include LCD System
#include "lcd20_4.h"



//
// PAGE SETUP
//
//
#define MENU_HEIGHT         1
#define PAGE_ROW_OFFSET_X   1
#define PAGE_ROW_MAX        3
#define PAGE_ROW_HEIGHT     1
#define SCREEN_WIDTH    20
#define SCREEN_HIGH    4

class MenuItem {
    
    public :

    MenuItem(String name, int activeMenuItem = 0) {
        _name = name;
        _activeMenuItem = activeMenuItem;
        _change = true;
        _currentMenuItem = false;
        _currentMenuItemHover = -1;
    }

    MenuItem(String name, bool currentMenuItem) {
      _name = name;
      _activeMenuItem = 0;
      _change = true;
      _currentMenuItem = currentMenuItem;
      _currentMenuItemHover = -1;
    }

    MenuItem(int type, String title, float (*dataValue)()){
      _type = type;
      _name = title;
      _dataItem = dataValue;
      _autorefresh = true;
    }

    MenuItem(int type, String title, String (*stringValue)()){
      _type = type;
      _name = title;
      _stringItem = stringValue;
      _autorefresh = true;
    }

    ~MenuItem() {
      delete _parentMenuItem;
    }


    

    public :

    //! Try to put cursor on down menu
    void on_menu_down();
    //! Try to put cursor on up menu
    void on_menu_up();
    //! Add a menu has child of current menu
    void add(MenuItem *menuItem);

    



    void activateMenuItem(){
      _currentMenuItem = true;
      _change = true;
    }
    void deActivateMenuItem(){
      _currentMenuItem = false;
      _change = false;
    }

    void enableChanged(bool enable = true){
      _change = enable;
    }
    void setChanged(bool enable = true){  _change = enable; }
    bool isChanged() const { return _change;  }

    int itemsCounter() {
      _menuItems.size();
    }


    void setName(String name) {
      _name = name;
    }
    String getName() const {
      return _name;
    }

    void setParent(MenuItem * parent) {
      _parentMenuItem = parent;
    }
    MenuItem * getParent() {
      return _parentMenuItem;
    }



    void setActivateMenuItem(int activateMenuItem) {
      _activeMenuItem = activateMenuItem;
    }
    int getActivateMenuItem() const {
      return _activeMenuItem;
    }

    void setCurrentMenuItem(bool isCurrentMenuItem = true) {
      _currentMenuItem = isCurrentMenuItem;
    }
    bool isCurrentMenuItem() const {
      return _currentMenuItem;
    }


    //! Getter/Setter _currentMenuItemHover
    int getCurrentMenuItemHover() const{  return _currentMenuItemHover; }
    void setCurrentMenuItemHover(int currentMenuItemHover = -1){  _currentMenuItemHover = currentMenuItemHover; }

    //! Getter/Setter _hover
    bool isHover() const { return _hover; }
    void setHover(bool hover = true){ _hover = hover; }

    //! Define type 0: menu 1: Data
    int getType() const{return _type; }
    //! Define type 0: menu 1: Data
    void setType(int type = 0){ _type = type; }

    void setAutoRefresh(bool enable = true){ _autorefresh = enable;  }
    bool getAutoRefresh() const{  return _autorefresh;  }


    MenuItem* operator=(MenuItem * menu){
      MenuItem* m = new MenuItem(menu->_name, menu->_currentMenuItem);
      m->setActivateMenuItem(menu->_activeMenuItem);
      m->setParent(menu->getParent());
      return m;
    }


    



    //=========================================================================
    //=========================================================================
    //
    // GETTER / SETTER
    //
    //=========================================================================
    //=========================================================================
    float getValue(){
      return _dataItem();
    }


    /**
     * @brief Get the Activate Menu Item object
     * 
     * @return LCDMenuItem* 
     */
    MenuItem *  getActivateMenuItem(){
      auto l_front = _menuItems.begin();
      if(_currentMenuItemHover<0){
        // check if parent available
        if(_parentMenuItem!=NULL){
          return _parentMenuItem;
        }
      }
      std::advance(l_front, _currentMenuItemHover);
      return (*l_front);
    }






    
    
    protected:
    /**
     * @brief _name is the current menu name. 
     * it should be display in the menu area name
     * 
     */
    String _name;
    
    /**
     * @brief activeMenuItem
     * 
     * Active menu item indicate which child is active as sub menu. 
     * if activeMenuItem equal 0 then mean no child is active menu. 
     * In this case, if the current menu (@see _currentMenuItem) is enable it 
     * show current item activated one otherwise nothing will append
     * @see _currentMenuItem
     */
    int _activeMenuItem;

    /**
     * @brief _change
     * Change is convenient way to know if cycle require to change the current
     * screen display. othewise last display remain as current. 
     * 
     */
    bool _change = false;

    /**
     * @brief _parentMenuItem
     * Is the parent menu item of the current menu. It is NULL when no parent 
     * exist. 
     * 
     */
    MenuItem *_parentMenuItem = NULL;

    /**
     * @brief _menuItems
     * is items menu of the current Menu. In another way it is a list of existing
     * child
     * 
     */
    std::list<MenuItem*> _menuItems;
    

    /**
     * @brief _currentMenuItemHover
     * 
     * Specify which menu item is hoverlay
     * when -1 mean cursor is on menu item
     * 
     */
    int _currentMenuItemHover;

    /**
     * @brief  _hover
     * Specify if actualy it should be defined has hover child
     */
    bool _hover = false;


    /**
     * @brief _currentMenuItem
     * Indicate that this menu is the current menu and should be display as the 
     * current one only if _activeMenuItem
     * 
     */
    bool _currentMenuItem = false;


    /**
     * @brief _page
     * Specify if it is current page
     * page is base on number of max row each time next row
     * is display page inc. 
     * 
     */
    // int _page = 0;
    int _page_row_start = 0;
    int _page_row_end   = 0;

    /**
     * @brief _pages
     * Specify the number of max page define while add some child
     * 
     */
    int _pages = 0;



    
      



    int _type = 0;   // 0: menu  1: Data  2: String Item
    String unit = ""; // unit in case of type data

    // Manage function
    typedef float (*DataItem)(void); // type for conciseness
    typedef String (*StringItem)(void); // 
    DataItem _dataItem; // variable to store function pointer type
    StringItem _stringItem; //
    //! Auto refresh for some child request
    bool _autorefresh = false;
};


class LCDMenuItem : public MenuItem {

  public :
    LCDMenuItem(String name, int activeMenuItem = 0) :MenuItem(name,activeMenuItem){
        _name = name;
        _activeMenuItem = activeMenuItem;
        _change = true;
        _currentMenuItem = false;
        _currentMenuItemHover = -1;
    }

    LCDMenuItem(String name, bool currentMenuItem) :MenuItem(name, currentMenuItem){
      _name = name;
      _activeMenuItem = 0;
      _change = true;
      _currentMenuItem = currentMenuItem;
      _currentMenuItemHover = -1;
    }

    LCDMenuItem(int type, String title, float (*dataValue)()): MenuItem(type, title, dataValue){
      _type = type;
      _name = title;
      _dataItem = dataValue;
      _autorefresh = true;
    }

    LCDMenuItem(int type, String title, String (*stringValue)()): MenuItem(type, title, stringValue){
      _type = type;
      _name = title;
      _stringItem = stringValue;
      _autorefresh = true;
    }


    void on_show(LCD20_4 *lcd);
    void on_show(LCD20_4 *lcd, int row);





        private:
    void drawMenu(LCD20_4 *lcd);
    void showScrollTop(LCD20_4 *lcd, boolean filled = true);
    void showScrollDown(LCD20_4 *lcd, boolean filled = true);






};

#endif //LCDMENUITEM_H