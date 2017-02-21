#ifndef MENU_H
#define MENU_H

#include "../Application.h"
#include "Button.h"
#include <vector>

using std::vector;

class Menu
{
protected:
	double m_mouse_x;
	double m_mouse_y;

	bool checkForHover(Button* btn);

public:
	//HACK
	vector <Button*> m_buttonList;

	//Adds a new button to the list
	inline void AddButton(Button* addButton) { m_buttonList.push_back(addButton); }

	//Sets if menu is interactable
	inline void SetActive(bool isActive) 
	{
		if (isActive)
			for (unsigned i = 0; i < m_buttonList.size(); ++i)
				m_buttonList[i]->SetActive(true);
		else
			for (unsigned i = 0; i < m_buttonList.size(); ++i)
				m_buttonList[i]->SetActive(false);
	}

	//Checks if buttons are pressed
	virtual void Update();

	//Renders the menu
	virtual void Render();

	Menu();
	virtual ~Menu();
};

class Shop_Menu : public Menu
{
protected:
	int *m_current_page;
	int m_num_item_per_page;

public:
	//Getters and Setters
	inline void SetCurrentPage(int &currentPage) { m_current_page = &currentPage; }
	inline void SetItemsPerPage(int item_per_page){ m_num_item_per_page = item_per_page; }
	inline int GetCurrentPage() { return *m_current_page; }
	inline int GetNumberOfItemsPerPage() { return m_num_item_per_page; }

	//Checks if buttons are pressed
	virtual void Update();

	//Renders the menu
	virtual void Render();

	virtual void Close(){};

	Shop_Menu() : m_current_page(0){};
	virtual ~Shop_Menu(){};
};

#endif