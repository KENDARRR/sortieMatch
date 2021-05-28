#include <bits/stdc++.h>
#include <iostream>
#include "patMatch.h"
#include <string>

using namespace std;

//Some list functions
List::List(int v){
	val = v;
	next = NULL;
}

List::~List(){
}

List* List::addT(int v){
	if(next == NULL){
		next = new List(v);
		return this;
	}
	next = next->addT(v);
	return this;
}

List* List::push(int v){ //Returns new head. Don't forget to catch it
	List* temp = new List(v);
	temp->next = this;
	return temp;
}

List* List::remove(int k){
	if(val == k){
		List* temp = next;
		delete this;
		return temp;
	}
	if(next){
		next = next->remove(k);
		return this;
	}
	return NULL;
}

List* List::pop(int* toFill){
	*toFill = val;	
	List *temp = next ? next : NULL;
	delete this;
	return temp;
}

int List::peek(){
	return val;
}

void List::printAll(){
	cout << val << " ";
	if(next){
		next->printAll();
	}
}
//

//--Evidently Structs have better performance than classes. I should try an alternate version with structs



//--Hoorspool's algorithm for comparison
int checkAt(string text, string pattern, int n){ //1 on success, -1 on fail.
	for(int i = 0; i < n; ++i){
		if(text[i] != pattern[i]){
			return -1;	
		}
	}
	return 1;
}

int hoorspool(string text, string pattern){
	int m = text.length();
	int n = pattern.length();
	if(m < n){
		return -1;
	}
	int tab[128];
	for(int i = 0; i < 128; ++i){
		tab[i] = n;
	}
	
	for(int i = 0; i < n-1; ++i){
		tab[pattern[i]] = (n-1) - i;
	}
	
	int s = 0;
	int ret = 0;
	
	while(s < m-n+1){
		ret = checkAt(&(text[s]), pattern, n);
		if(ret == 1){
			return s;
		}
		s += tab[text[s+n-1]];
	}
	
	return -1;
}

//-- V.1 for preparation of alphabet list
//-- Uses very inefficient add to tail, where each add traverses the list you're adding to.
//-- I'm lazy, so instead of fixing my data structures, I'm just going to add everything at the head; but add them in reverse order. heh...
List** prep(int *itab, string t){
	List** tab = new List*[126];  //Alphabet lists
	
	//get lengths
	int nt = t.length();
	//Prep: read text into the alphabet lists.
	for(int i = 0; i < nt; ++i){
		if(tab[t[i]] == NULL){
			List* temp = new List(i);
			tab[t[i]] = temp;
		} else {
			tab[t[i]] = (tab[t[i]])->addT(i); //TODO: Currently does very inefficient addT. Add an actual tail pointer to improve prep speed.
		}
		++(itab[t[i]]);
	}
	//Print
	/*
	for(int j = 0; j < 126; ++j){
		if(tab[j]){
			cout << char(j) << ": ";
			tab[j]->printAll();
			cout << endl;
		}
	}
	*/
	//End Print
	return tab;
}

//-- V.2 for preparation of alphabet list
//-- Again, this just adds to the head, but traverses backwards. Should work the same though.
//-- This will make it so we can use the far more efficient List::pop(int v), and cut down on prep time.
List** prep_plus(int *itab, string t, int nt){
	List** tab = new List*[126];  //Alphabet lists
	//get lengths
	//Prep: read text into the alphabet lists.
	for(int i = nt; i >= 0; --i){
		if(tab[t[i]] == NULL){
			List* temp = new List(i);
			tab[t[i]] = temp;
		} else {
			tab[t[i]] = (tab[t[i]])->push(i); 
		}
		++(itab[t[i]]);
	}
	//Print
	/*
	for(int j = 0; j < 126; ++j){
		if(tab[j]){
			cout << char(j) << ": ";
			tab[j]->printAll();
			cout << endl;
		}
	}
	*/
	//End Print
	return tab;
}

 
int end_sortie(string text, string pat, List** tab){
	int np = pat.length();
	int nt = text.length();
	List* sp = tab[pat[0]];
	int sp_val;

	while(sp){
		sp = tab[pat[0]] = (tab[pat[0]])->pop(&sp_val);
		bool flag = true;
		for(int i = 1; i < np; ++i){
			if(text[sp_val + i] != pat[i]){
				flag = false;
				break;
			}
		}
		if(flag){
			return sp_val;
		}
	}
	return -1;
}

int end_sortie_plus(string text, string pat, List** tab, int* itab, int np, int nt){
	List* sp = tab[pat[0]];
	int sp_val;
	int in = 0;
	int min = itab[pat[0]];
	for(int i = 1; i < np; ++i){
		if(itab[i] < min){
			min = itab[i];
			in = i;
		}
	}

	while(sp){
		sp = tab[pat[0]] = (tab[pat[0]])->pop(&sp_val);
		bool flag = true;
		for(int i = 1; i < np; ++i){
			if(text[sp_val + i] != pat[i]){
				flag = false;
				break;
			}
		}
		if(flag){
			return sp_val;
		}
	}
	return -1;
}


//-- 1st prototype of algorithm, kinda messy
int sortie(string text, string pat){
	List* tab[126];  //Alphabet lists
	int itab[126];
	for(int i = 0; i < 126; ++i){
		tab[i] = NULL;
		itab[i] = 0;
	}
	
	//get lengths
	int np = pat.length();
	int nt = text.length();
	
	//Prep: read text into the alphabet lists.
	for(int i = 0; i < nt; ++i){
		if(tab[int(text[i])] == NULL){
			List* temp = new List(i);
			tab[(text[i])] = temp;
		} else {
			tab[(text[i])] = (tab[(text[i])])->addT(i);
		}
		++(itab[text[i]]);
	}

	//Print
	/*
	for(int i = 0; i < 126; ++i){
		if(tab[i]){
			cout << char(i) << ": ";
			tab[i]->printAll();
			cout << endl;
		}
	}
	**/
	//End Print

	//Search for pattern. 
/*	//---Find shortest stack, compare from there. 
 *	//---This would be more efficient, but is a touch more complicated than starting from the first character. Also, evidently compilers make array access and pointer dereferencing take the same amount of time. So maybe don't worry about that part too much.
	}
	*/
	//Find shortest 
	int in = 0;
	int min = itab[pat[0]];
	for(int i = 1; i < np; ++i){
		if(itab[i] < min){
			min = itab[i];
			in = i;
		}
	}
	cout << "in = " << in << endl;
	//Easy search
	List* sp = tab[pat[in]];
	List* tp;
	int sp_val;
	while(sp){
		sp = tab[pat[in]] = (tab[pat[in]])->pop(&sp_val);
		bool flag = true;
		for(int i = 0; i < np; ++i){
			if(text[i + sp_val-in] != pat[i]){
				flag = false;
				break;
			}
		}
		if(flag){
			return sp_val-in;
		}
	}
	return -1;
}


int main(){
	string text;
	string pat;
	cout << "Enter the text: \n";
	cin >> text;
	cout << "Enter the pattern: \n";
	cin >> pat;
	clock_t s, e;
	double tt;
	int *itab = new int[126];
	List ** ret = prep(itab, text);
	int r;
	int pl;
	int tl;
	
	cout << "////////\nTest for sortie pattern matching algorithm:" << endl;
	s = clock();	
	r = end_sortie(text, pat,ret);
	e = clock();
	tt = (double(e - s) /double(CLOCKS_PER_SEC));
	cout << "At index : " << r << endl;
	cout << "Time taken : " << fixed << tt;
	cout << " sec(s)." << endl;

	List ** pro = prep(itab, text);
	cout << "////////\nTest for sortie PLUS pattern matching algorithm:" << endl;
	
	s = clock();
	pl = pat.length();
	tl = text.length();
	r = end_sortie_plus(text, pat, pro, itab, pl, tl);
	e = clock();
	tt = (double(e - s) /double(CLOCKS_PER_SEC));
	cout << "At index : " << r << endl;
	cout << "Time taken : " << fixed << tt;
	cout << " sec(s)." << endl;

	cout << "////////\nTest for sortie PLUS + prep pattern matching algorithm:" << endl;
	s = clock();
	List ** propie = prep(itab, text);
	pl = pat.length();
	tl = text.length();
	r = end_sortie_plus(text, pat, propie, itab, pl, tl);
	e = clock();
	tt = (double(e - s) /double(CLOCKS_PER_SEC));
	cout << "At index : " << r << endl;
	cout << "Time taken : " << fixed << tt;
	cout << " sec(s)." << endl;

	cout << "////////\nTest for sortie PLUS + prep PLUS pattern matching algorithm:" << endl;
	List ** propit;
	s = clock();
	pl = pat.length();
	tl = text.length();
	propit = prep_plus(itab, text, tl);
	r = end_sortie_plus(text, pat, propit, itab, pl, tl);
	e = clock();
	tt = (double(e - s) /double(CLOCKS_PER_SEC));
	cout << "At index : " << r << endl;
	cout << "Time taken : " << fixed << tt;
	cout << " sec(s)." << endl;

	cout << "////////\nTest for Horspool's pattern matching algorithm:" << endl;
	s = clock();	
	r = hoorspool(text, pat);
	e = clock();
	tt = (double(e - s) /double(CLOCKS_PER_SEC));
	cout << "At index : " << r << endl;
	cout << "Time taken : " << fixed << tt;
	cout << " sec(s)." << endl;
	return 0;
}










