#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std; 

struct trie_node{
	bool NotLeaf;
	char word[20];
	char def[20];
	trie_node *pChildren[27];
};
typedef struct trie_node* TRIE;

TRIE NewLeaf (char keyWord[20], char defn[20]) {
	TRIE t_node;
	t_node = (TRIE) malloc (sizeof(struct trie_node));

	for (int count = 0; count < 27; count++)
		t_node -> pChildren[count] = NULL;

	t_node -> NotLeaf = false;
	strcpy (t_node -> word, keyWord);
	strcpy (t_node -> def, defn);

	return t_node;
}

TRIE NewIntern () {
	TRIE t_node;
	t_node = (TRIE) malloc (sizeof(struct trie_node));

	for (int count = 0; count < 27; count++)
		t_node -> pChildren[count] = NULL;

	t_node -> NotLeaf = true;
	t_node -> word[0] = 0;

	return t_node;
}

int Find (trie_node *trie, char keyWord[20]) {
	TRIE next, index, data;
	int count;

	next = trie;

	if (next == NULL)
		cout << "Word Not found" << endl;
	else
		index = next;
		
	count = 0;

	while ((index -> NotLeaf == true) && (count < strlen(keyWord))
		&& (index -> pChildren[keyWord[count] - 'a'] != NULL)) {

		next = index -> pChildren[keyWord[count] - 'a'];
		index = next;
		count++;
	}

	if (next == NULL) {
		cout << "Word Not Found in this Dictionary" << endl;
		return 0;
	} else {
		data = next;

		if (!strcmp (data -> word, keyWord)) {
			cout << "Meaning of " << keyWord << " : " << data -> def << endl;
			return 1;
		} else {
			if ((data -> pChildren[26]) &&  !strcmp(data -> pChildren[26] -> word, keyWord)) {
				cout << "Meaning of " << keyWord << " : " << data -> def << endl;
				return 1;
			}
			else {
				cout << "Word Not found in this Dictionary" << endl;
				return 0;
			}
		}
	}
}

TRIE insert (TRIE trie, char keyWord[20], char defn[20]) {
	TRIE next, parent, index;
	TRIE new_leaf, data, new_index;
	TRIE oldChildren, new_Word, intern;

	int inWordIndex, prefixLength, length = strlen (keyWord);

	next = trie;

	if (next == NULL) {
		trie = NewIntern ();
		new_leaf = NewLeaf (keyWord, defn);
		trie -> pChildren[keyWord[0] - 'a'] = new_leaf;

		return trie;
	} else {
		index = next;
	}

	inWordIndex = 0;

	while ((inWordIndex < length) && (index -> NotLeaf == true)
		&& (index -> pChildren[keyWord[inWordIndex] - 'a'] != NULL)) {

		parent = next;
		next = index -> pChildren[keyWord[inWordIndex] - 'a'];
		index = next;

		inWordIndex++;
	}

	if ((inWordIndex < length) && (index->pChildren[keyWord[inWordIndex]-'a'] == NULL) 
		&& (index -> NotLeaf == true)) {

		new_index = NewLeaf (keyWord, defn);
		index -> pChildren[keyWord[inWordIndex] - 'a'] = new_index;

		return trie;
	} else {
		data = next;
	}

	if (!strcmp(data -> word, keyWord)) {
		cout << "word already exists in trie" << endl;
	} else {
		oldChildren = parent -> pChildren[keyWord[inWordIndex - 1] - 'a'];
		new_Word = NewLeaf (keyWord, defn);

		prefixLength = strlen (keyWord);

		if (data -> word[0] != '\0')
			if (strlen (data -> word) < prefixLength)
				prefixLength = strlen (data -> word);
	}

	bool createIntern = false;

	while ((inWordIndex <= prefixLength) && ((( data -> word[0] != '\0')
		&& (data -> word[inWordIndex - 1] == keyWord[inWordIndex - 1])) || (data -> word[0] == '\0'))) {

		intern = NewIntern ();
		parent -> pChildren[keyWord[inWordIndex - 1] - 'a'] = intern;
		parent -> NotLeaf = true;
		parent = intern;

		inWordIndex++;
		createIntern = true;
	}

	if (createIntern)
		inWordIndex--;

	if ((strlen(keyWord) == strlen(data -> word))) {

		parent -> pChildren[data -> word[inWordIndex] - 'a'] = oldChildren;
		parent -> pChildren[keyWord[inWordIndex] - 'a'] = new_Word;
	} else {
		if (data -> word[0] != '\0') {
			if (strlen (data -> word) <= prefixLength) {

				parent -> pChildren[26] = oldChildren;
				parent -> pChildren[keyWord[prefixLength] - 'a'] = new_Word;
			} else {

				parent -> pChildren[26] = new_Word;
				parent -> pChildren[data -> word[prefixLength] - 'a'] = oldChildren;
			}
		} else {
			for (int count = 0; count < 27; count++)
				parent -> pChildren[count] = oldChildren -> pChildren[count];

			parent -> pChildren[26] = new_Word;
		}
	}

	return trie;
}

int main () {
	TRIE trie;
	trie = NULL;

	int x;
	char str[20];
	char def[20];

	fstream dict;
	dict.open ("dict.dat", ios :: in | ios :: binary);

	while (!dict.eof()) {
		dict >> str >> def;
		trie = insert (trie, str, def);
	}

	int n = 1;
	while (n != 2) {
		cout << "\n" << "1. Search for Meaning of the word" << endl << "2. Exit" << endl << endl;
		cout << "Enter your choice : ";
		cin >> n; cout << endl;

		if (n == 1) {
			cout << "Enter the word : ";
			cin >> str;

			x = Find (trie, str);
			if (x == 0) {
				cout << "\n" << "Enter the meaning of " << str << " : ";
				cin >> def;

				fstream dict;
				dict.open ("dict.dat", ios :: app | ios :: out | ios :: binary);

				dict << "\n" << str << " " << def << endl;

				trie = insert (trie, str, def);

				dict.close ();
			}
		} cout << endl;
	} 

	return 0;
}
