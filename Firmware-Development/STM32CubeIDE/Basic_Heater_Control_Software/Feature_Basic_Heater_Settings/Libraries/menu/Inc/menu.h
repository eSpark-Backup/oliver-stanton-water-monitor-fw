#pragma once

struct level {
   char name[16];
   struct level *next;
   struct level *prev;
   struct level *down;
   struct level *up;
   void (*DoWork)(void);
};
class Menu{
public:
	int numberOfItems;
	void BuildMenu(struct level *currentNode, char name[16], void (*DoWork)(void) , struct level *prevNode, struct level *nextNode,struct level *upNode,struct level *downNode);
	void createMenu(const char** menu);
	void arrowDown();
	void arrowUp();
	void displayArrow();
	void next();
};





