#include "AST.h"
element * AST::getRoot()
{
	return root;
}
void AST::setRoot(element * node)
{
	root = node;
}
void AST::TreeWalk(element* pItem)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}