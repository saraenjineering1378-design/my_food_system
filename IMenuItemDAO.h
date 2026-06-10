#ifndef IMENU_ITEMDAO_H
#define IMENU_ITEMDAO_H

#include <vector>
#include "MenuItem.h"

class IMenuItemDAO
{
public:

   
   virtual MenuItem* findMenuItemById(int id) = 0;
   virtual std::vector<MenuItem*> getMenuItemsByRestaurant(int restaurantId) = 0;
   virtual ~IMenuItemDAO() {} 
   virtual std::vector<MenuItem*> getAllMenuItems() = 0;
   virtual bool addMenuItem(int restaurantId, MenuItem* item) = 0;
   virtual void updateMenuItem(MenuItem* item) = 0;
   virtual void removeMenuItem(int id) = 0;
   

};
#endif  
