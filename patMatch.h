#ifndef _PATTERNMATCHING_
#define _PATTERNMATCHING_


class List{
	protected:
		int val;
		List *next;

	public:
		List(int v);
		~List();
		List* addT(int v);
		List* push(int v);
		List* remove(int k);
		List* pop(int* toFill);
		int peek();
		void printAll();
};







#endif
