#include "LCDMenuItem.hpp"

/**
 * @brief 
 * 
 * @param lcd 
 * @param row 
 */
void LCDMenuItem::on_show(LCD20_4 *lcd, int row) {


  // Depend on current row selected so child _over is on
  if(_hover){
    lcd->getLCD()->setCursor(0, row);   
    lcd->getLCD()->print(">");
    lcd->getLCD()->cursor();
  }

  //
  // Print text
  //
  lcd->getLCD()->setCursor(1, row);                 // Start at top-left corner
  if(_type==0){
    lcd->getLCD()->println(_name);
    dbg("On row = " + String(row) + " name : " + String(_name));
  }else if(_type==1){
    lcd->getLCD()->println(_name + String(_dataItem()));
  }else if(_type==2){
    lcd->getLCD()->println(_name + String(_stringItem()));
  }
  
}


/**
 * @brief on_show
 * Main mechanism to check how to display content of current page
 * 
 * @param lcd displayer support
 */
void LCDMenuItem::on_show(LCD20_4 *lcd) {

  // stop if no change or currentMenuItem is not enable
  if(_autorefresh)
    _change= true;

  if(!_change or !_currentMenuItem)  return;


  // Clear the display
  lcd->lclear();

  // Draw Menu
  drawMenu(lcd);


  std::list<MenuItem*>::iterator it;
  int r = 0;
  int skip = 0;
  for (it = _menuItems.begin(); it != _menuItems.end(); ++it) {
    if(skip >= _page_row_start and skip <= _page_row_end){
      (*it)->setHover(false);
      if(r + _page_row_start==_currentMenuItemHover){
        (*it)->setHover(true);
      }
      LCDMenuItem *a = (LCDMenuItem*)(*it);
      a->on_show(lcd, r+1);
      r = r + 1; 
    }
    skip = skip + 1;
  }


  //
  // Manage scroll display 
  //
  if(_page_row_start == 0){
    showScrollTop(lcd, false);
  }else{
    showScrollTop(lcd, true);
  }
  if(_page_row_start < _pages){
    showScrollDown(lcd, true);
  }else{
    showScrollDown(lcd, false);
  }
  

  // Disable change appeared
  _change = false;

}














/**
 * @brief  Try to put cursor on down menu
 * 
 * 
 */
void MenuItem::on_menu_down(){
  int size = _menuItems.size();
  if(_currentMenuItemHover >= size-1){
    return;
  }
  
  _currentMenuItemHover = _currentMenuItemHover + 1;

  if(_currentMenuItemHover >= _page_row_end){
    _page_row_start = _page_row_start + _currentMenuItemHover / 4; // _page + 
    _page_row_end = _page_row_start + PAGE_ROW_MAX;
  }


  this->_change = true;
}


/**
 * @brief  Try to put cursor on up menu
 * 
 * 
 */
void MenuItem::on_menu_up(){
  if(_currentMenuItemHover <= -1){
    return;
  }
  _currentMenuItemHover = _currentMenuItemHover - 1;

  if(_currentMenuItemHover < _page_row_start and _currentMenuItemHover >=0 and _page_row_start > 0){
    _page_row_start--;
    _page_row_end = _page_row_start + PAGE_ROW_MAX;
  }

  this->_change = true;
}


/**
 * @brief 
 * 
 * @param menuItem 
 */
void MenuItem::add(MenuItem *menuItem) {

  menuItem->setParent(this);

  _menuItems.push_back(menuItem);
  int size = _menuItems.size();

  // Compute end of page displayable at start
  _page_row_end = size;
  if(_page_row_end > PAGE_ROW_MAX){
    _page_row_end = PAGE_ROW_MAX;
  }

  _pages = size - PAGE_ROW_MAX;
}













/**
 * @brief drawMenu
 * 
 * 
 * @param menu 
 */
void LCDMenuItem::drawMenu(LCD20_4 *lcd){

  //
  if(_currentMenuItemHover==-1){
    lcd->getLCD()->setCursor(0, 0);
    lcd->getLCD()->print(">");
    lcd->getLCD()->cursor();
  }

  //
  lcd->getLCD()->setCursor(1,0);               // Start at top-left corner
  lcd->getLCD()->print(_name);

  //
  if(_currentMenuItemHover==-1){
    lcd->getLCD()->setCursor(19, 0);
    lcd->getLCD()->write((byte)0);
  }

}

/**
 * @brief 
 * 
 * @param oled 
 * @param filled 
 */
void LCDMenuItem::showScrollTop(LCD20_4 *lcd, boolean filled){
//   int ox = 0, oy = MENU_HEIGHT + (0 * PAGE_ROW_HEIGHT);
//   if(filled)
//     lcd->fillTriangle(ox, oy+5, ox+3, oy+1, ox+6, oy+5, SSD1306_WHITE);
//   else
//     lcd->drawTriangle(ox, oy+5, ox+3, oy+1, ox+6, oy+5, SSD1306_WHITE);

}


/**
 * @brief 
 * 
 * @param oled 
 * @param filled 
 */
void LCDMenuItem::showScrollDown(LCD20_4 *oled, boolean filled){
//   int ox = 0, oy = MENU_HEIGHT + (3 * PAGE_ROW_HEIGHT);
//   if(filled)
//     oled->fillTriangle(ox, oy+5, ox+3, oy+9, ox+6, oy+5, SSD1306_WHITE);
//   else
//     oled->drawTriangle(ox, oy+5, ox+3, oy+9, ox+6, oy+5, SSD1306_WHITE);

}
