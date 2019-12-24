#include <iostream>
#include <fstream>
#include "DateTime.h"
#include "Headers.h"
#include "Items.h"
#include "ICS0017DataSource.h"
#include <string.h>
#pragma warning (disable : 4996)

using namespace std;

void PrintDataStructure(HEADER_A **pStruct3) {
	int n = 1;
	for (int i = 0; i < 26; i++) {
		HEADER_A* temp_head = pStruct3[i];
		for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
			ITEM6* temp_item = (ITEM6*)temp_head->pItems;
			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				printf("%d)%s %d %d:%d:%d\n",n, temp_item->pID, temp_item->Code, temp_item->Date.Day, temp_item->Date.Month, temp_item->Date.Year);
				n++;
			}
		}
	}
}

HEADER_A** InsertItem(HEADER_A** pStruct3, char* pNewItemID = 0) {

	ITEM6* pNewItem = (ITEM6*)malloc(sizeof(ITEM6));

	if (pNewItemID != 0) {
		int spaces=0, forbiddenChars=0;
		for (size_t i = 0; i<strlen(pNewItemID); i++) {
			if (pNewItemID[i] == ' ') {
				spaces++;
			}
			if ((pNewItemID[i]<65 && pNewItemID[i]!=45 && pNewItemID[i]!=32) || (pNewItemID[i]>90 && pNewItemID[i]<97) || pNewItemID[i]>122) {
				forbiddenChars++;
			}
		}

		if (spaces != 1) {
			printf("There should be 1 and only 1 space!\n");
		}
		else if (forbiddenChars > 0) {
			printf("You used characters which are not allowed!\n");
		}
		else if (strlen(pNewItemID)<3) {
			printf("There's a word missing!\n");
		}
		else {
			char* pch = (char*)malloc((size_t)pNewItemID);
			strcpy(pch, pNewItemID);
			char* id1 = strtok(pch," ");
			char* id2 = strtok(NULL," ");

			if(id1[0]<65 || id1[0]>90 || id2[0]<65 || id2[0]>90){
				printf("UPPERCASE IS MISSING!\n");
			}
			else {
				int pHead = (int)pNewItemID[0] - 65;
				HEADER_A* temp = pStruct3[pHead];
				bool shouldBreak = 0;
				
				for (temp; temp != 0; temp = (temp->pNext)) {
					ITEM6* temp_item = (ITEM6*)temp->pItems;

					for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
						char* tempId = temp_item->pID;
						if (!strcmp(tempId, pNewItemID)) {
							printf("There already exists a matching ID\n");
							shouldBreak = 1;
							break;
						}
					}
					if (shouldBreak) break;
				}

				if (shouldBreak == 0) {
					pNewItem = (ITEM6*)GetItem(6, pNewItemID);
				}
			}
		}
	}else {
		pNewItem = (ITEM6*)GetItem(6);
	}
	


	char* item_id = (char*)malloc(sizeof(pNewItem->pID));
	strcpy(item_id, pNewItem->pID);
	char* item_id1 = strtok(item_id, " ");
	char* item_id2 = strtok(NULL, " ");



	if (pStruct3[item_id[0] - 65] != 0) {

		HEADER_A* temp = pStruct3[item_id[0] - 65];
		HEADER_A* pPrev = 0;

		for (temp; temp != 0; temp = temp->pNext) {
			if (item_id2[0] == temp->cBegin) {
				ITEM6* pOldItem = (ITEM6*)temp->pItems;
				temp->pItems = (void*)pNewItem;
				ITEM6* pCurrentItem = (ITEM6*)temp->pItems;
				pCurrentItem->pNext = pOldItem;
				break;
			}
			else if (item_id2[0] > temp->cBegin && temp->pNext != 0) {
				pPrev = temp;
			}
			else if (item_id2[0] > temp->cBegin && temp->pNext == 0) {
				HEADER_A* my_head = (HEADER_A*)malloc(sizeof(HEADER_A));
				my_head->cBegin = item_id2[0];
				my_head->pItems = (void*)pNewItem;
				my_head->pNext = 0;

				temp->pNext = my_head;
				break;
			}
			else if (item_id2[0] < temp->cBegin) {
				HEADER_A* my_head = (HEADER_A*)malloc(sizeof(HEADER_A));
				my_head->cBegin = item_id2[0];
				my_head->pItems = (void*)pNewItem;

				if (pPrev != 0) {
					pPrev->pNext = my_head;
					my_head->pNext = temp;
				}
				else{
					HEADER_A* nextP = temp;
					pStruct3[item_id[0]-65] = my_head;
					my_head->pNext = nextP;
				}
				break;
			}
		}
	}
	else {
		HEADER_A* my_head = (HEADER_A*)malloc(sizeof(HEADER_A));
		my_head->cBegin = item_id2[0];
		my_head->pItems = (void*)pNewItem;
		my_head->pNext = 0;

		pStruct3[item_id[0] - 65] = my_head;
	}



	return pStruct3;

}

HEADER_A** RemoveItem(HEADER_A** pStruct3, char* pItemID) {
	if (pItemID != 0) {
		int spaces = 0, forbiddenChars = 0;
		for (size_t i = 0; i < strlen(pItemID); i++) {
			if (pItemID[i] == ' ') {
				spaces++;
			}
			if ((pItemID[i] < 65 && pItemID[i] != 45 && pItemID[i] != 32) || (pItemID[i] > 90 && pItemID[i] < 97) || pItemID[i] > 122) {
				forbiddenChars++;
			}
		}

		if (spaces != 1) {
			printf("There should be 1 and only 1 space!\n");
		}
		else if (forbiddenChars > 0) {
			printf("You used characters which are not allowed!\n");
		}
		else if (strlen(pItemID) < 3) {
			printf("There's a word missing!\n");
		}
		else {
			char* pch = (char*)malloc((size_t)pItemID);
			strcpy(pch, pItemID);
			char* id1 = strtok(pch, " ");
			char* id2 = strtok(NULL, " ");

			if (id1[0] < 65 || id1[0]>90 || id2[0] < 65 || id2[0]>90) {
				printf("UPPERCASE IS MISSING!\n");
			}
			else {
				int pHead = (int)pItemID[0] - 65;
				HEADER_A* temp = pStruct3[pHead];
				bool shouldBreak = 0;
				bool isItem = 0;

				ITEM6* pCurrentItem = 0;
				ITEM6* pPrev_item = 0;
				ITEM6* nextP_item = 0;

				HEADER_A* pCurrentHead = 0;
				HEADER_A* pPrev_head = 0;
				HEADER_A* nextP_head = 0;

				for (temp; temp != 0; temp = (temp->pNext)) {
					ITEM6* temp_item = (ITEM6*)temp->pItems;

					for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
						char* tempId = temp_item->pID;
						if (!strcmp(tempId, pItemID)) {
							pCurrentItem = temp_item;
							nextP_item = pCurrentItem->pNext;
							pCurrentHead = temp;
							nextP_head = temp->pNext;

							isItem = 1;
							shouldBreak = 1;
							break;
						}
						else {
							pPrev_item = temp_item;
							pPrev_head = temp;
						}
					}
					if (shouldBreak) break;
					pPrev_item = 0;
				}

				if (isItem) {
					if (pPrev_item != 0) {
						pPrev_item->pNext = nextP_item;
						delete pCurrentItem;
						printf("Removed %s\n", pItemID);
					}
					else if (nextP_item != 0) {
						pCurrentHead->pItems = (void*)nextP_item;
					}
					else{
						if (pPrev_head != 0) {
							pPrev_head->pNext = nextP_head;
							delete pCurrentItem;
							delete pCurrentHead;
						}
						else if(nextP_head != 0){
							delete pCurrentItem;
							delete pCurrentHead;
							pStruct3[pHead] = nextP_head;
						}
						else {
							delete pCurrentItem;
							delete pCurrentHead;
							pStruct3[pHead] = 0;
						}
					}
				}
				else {
					printf("There isn't a matching ID\n");
				}
			}
		}
	}

	return pStruct3;
}

////////////////////////////////////////////////////////////////////////////

class DataStructure {
public:
	HEADER_A** ppStruct3;
	DataStructure();
	~DataStructure();
	DataStructure(const DataStructure& Original);
	DataStructure(char* pFileName);
	int GetItemsNumber(HEADER_A** pStruct3);
	ITEM6* GetItem(char* pID, HEADER_A** pStruct3);
	ITEM6* GetItem(long int code, HEADER_A** pStruct3);
	ITEM6* GetItem(DATE1 d, HEADER_A** pStruct3);
	void operator+=(ITEM6* item);
	void operator-=(char* pItemID);
	int operator==(DataStructure &Other);
	DataStructure& operator=(const DataStructure& Right);
	void Write(char* pFileName);


	friend std::ostream& operator<<(std::ostream& ostr, const DataStructure& str) {
		
		PrintDataStructure(str.ppStruct3);

		return ostr;
	}

};

DataStructure::DataStructure() {

	ppStruct3 = (HEADER_A**)malloc(sizeof(HEADER_A*)*26);

	for (int i = 0; i < 26; i++) {
		ppStruct3[i] = 0;
	}
}

DataStructure::~DataStructure(){

	printf("Destructor Running!\n");

	for (int i = 0; i < 26; i++) {

		HEADER_A* pPrevHead = 0;

		if (ppStruct3[i] != 0) {

			HEADER_A* temp_head = ppStruct3[i];
			for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
				delete pPrevHead;

				ITEM6* pPrevItem = 0;
				ITEM6* temp_item = (ITEM6*)temp_head->pItems;
				for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {

					delete pPrevItem;
					delete (temp_item->pID);

					if (temp_item->pNext == 0) {
						delete temp_item;
						break;
					}
					else {
						pPrevItem = new ITEM6;
						pPrevItem = temp_item;
					}
				}

				if (temp_head->pNext == 0) {
					delete temp_head;
					break;
				}
				else {
					pPrevHead = new HEADER_A;
					pPrevHead = temp_head;
				}
			}
		}
	}
	
	delete ppStruct3;
}

DataStructure::DataStructure(const DataStructure& Right) {
	ppStruct3 = new HEADER_A * [26];

	for (int i = 0; i < 26; i++) {
		ppStruct3[i] = new HEADER_A;
		HEADER_A* right_head = Right.ppStruct3[i];
		HEADER_A* left_head = ppStruct3[i];

		if (right_head == 0) {
			ppStruct3[i] = 0;
		}
		else {

			for (right_head; right_head != 0; right_head = right_head->pNext) {
				left_head->cBegin = right_head->cBegin;

				left_head->pItems = (void*)malloc(sizeof(ITEM6));
				ITEM6* left_item = (ITEM6*)left_head->pItems;
				ITEM6* right_item = (ITEM6*)right_head->pItems;

				for (right_item; right_item != 0; right_item = right_item->pNext) {
					left_item->Code = right_item->Code;
					left_item->Date = right_item->Date;
					left_item->pID = new char[strlen(right_item->pID) + 1];
					strcpy(left_item->pID, right_item->pID);

					left_item->pNext = new ITEM6;
					if (right_item->pNext == 0) {
						left_item->pNext = 0;
					}
					else {
						left_item = left_item->pNext;
					}
				}

				left_head->pNext = new HEADER_A;
				if (right_head->pNext == 0) {
					left_head->pNext = 0;
				}
				else {
					left_head = left_head->pNext;
				}

			}
		}
	}

	/*for (int i = 0; i < 26; i++) {
		ppStruct3[i] = new HEADER_A;

		HEADER_A* cp_temp = ppStruct3[i];
		HEADER_A* og_temp = Original.ppStruct3[i];

		if (Original.ppStruct3[i] != 0) {
			for (og_temp; og_temp != 0; og_temp = og_temp->pNext) {
				cp_temp->cBegin = og_temp->cBegin;

				cp_temp->pNext = new HEADER_A;
				if (og_temp->pNext == 0) { cp_temp->pNext = 0; }

				cp_temp->pItems = (void*)malloc(sizeof(ITEM6));
				ITEM6* cp_item = (ITEM6*)cp_temp->pItems;
				ITEM6* og_item = (ITEM6*)og_temp->pItems;

				for (og_item; og_item != 0; og_item = og_item->pNext) {
					cp_item->Code = og_item->Code;

					cp_item->Date = og_item->Date;

					cp_item->pID = new char[sizeof(og_item->pID) + 1];
					strcpy(cp_item->pID, og_item->pID);

					cp_item->pNext = new ITEM6;
					if (og_item->pNext == 0) { cp_item->pNext = 0; }
					cp_item = cp_item->pNext;

				}

				cp_temp = cp_temp->pNext;
			}
		}
		else {
			ppStruct3[i] = 0;
		}
	}*/

}

DataStructure::DataStructure(char* pFileName) {

	ppStruct3 = new HEADER_A * [26];
	for (int i = 0; i < 26; i++) {
		ppStruct3[i] = 0;
	}

	fstream File;

	File.open(pFileName, fstream::in | fstream::binary);

	File.seekg(0, fstream::end);
	int size = (int)File.tellg();
	File.seekg(0, fstream::beg);
	//cout << "The file is " << size << " bytes" << endl;

	char cBegin;
	char* pID;
	unsigned long int code;
	DATE1 date;
	size_t len = 0;

	while (File.tellg() < size) {

		File.read((char*)&cBegin, sizeof(char));
		File.read((char*)&code, sizeof(unsigned long int));
		File.read((char*)&date.Day, sizeof(int));
		File.read((char*)&date.Month, sizeof(int));
		File.read((char*)&date.Year, sizeof(int));

		File.read((char*)&len, sizeof(len));
		pID = new char[len];
		File.read(pID, len);
		pID[len] = '\0';



		InsertItem(ppStruct3, pID);
		ITEM6* my_item = GetItem(pID, ppStruct3);
		my_item->Code = code;
		my_item->Date.Day = date.Day;
		my_item->Date.Month = date.Month;
		my_item->Date.Year = date.Year;

	}

	File.close();
}

int DataStructure::GetItemsNumber(HEADER_A** pStruct3) {
	int n = 0;

	for (int i = 0; i < 26; i++) {

		HEADER_A* temp = pStruct3[i];

		for (temp; temp != 0; temp = temp->pNext) {
			ITEM6* temp_item = (ITEM6*)temp->pItems;

			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				n++;
			}
		}
	}

	return n;
}

ITEM6* DataStructure::GetItem(char* pID, HEADER_A** pStruct3) {

	for (int i = 0; i < 26; i++) {

		HEADER_A* temp = pStruct3[i];

		for (temp; temp != 0; temp = temp->pNext) {
			ITEM6* my_item = (ITEM6*)temp->pItems;
			ITEM6* temp_item = my_item;

			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				if (!strcmp(pID, temp_item->pID)) {
					return temp_item;
				}
			}
		}
	}

	return 0;
}

ITEM6* DataStructure::GetItem(long int code, HEADER_A** pStruct3) {

	for (int i = 0; i < 26; i++) {

		HEADER_A* temp = pStruct3[i];

		for (temp; temp != 0; temp = temp->pNext) {
			ITEM6* my_item = (ITEM6*)temp->pItems;
			ITEM6* temp_item = my_item;

			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				if (code == temp_item->Code) {
					return temp_item;
				}
			}
		}
	}

	return 0;
}

ITEM6* DataStructure::GetItem(DATE1 d, HEADER_A** pStruct3) {

	for (int i = 0; i < 26; i++) {

		HEADER_A* temp = pStruct3[i];

		for (temp; temp != 0; temp = temp->pNext) {
			ITEM6* my_item = (ITEM6*)temp->pItems;
			ITEM6* temp_item = my_item;

			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				if (d.Day == temp_item->Date.Day && d.Month == temp_item->Date.Month && d.Year == temp_item->Date.Year) {
					return temp_item;
				}
			}
		}
	}

	return 0;
}

void DataStructure::operator+=(ITEM6* item) {

	char* item_id = new char [sizeof(item->pID)];
	strcpy(item_id, item->pID);
	char* id1 = strtok(item_id, " ");
	char* id2 = strtok(NULL, " ");

	HEADER_A* temp_head = ppStruct3[id1[0] - 65];
	for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
		ITEM6* temp_item = (ITEM6*)temp_head->pItems;

		for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
			if (!strcmp(item->pID, temp_item->pID)) {
				printf("ID already exitst in the structure!\n");
				return;
			}
		}
	}
	temp_head = ppStruct3[id1[0] - 65];

	if (temp_head != 0) {
		HEADER_A* pPrev = 0;

		for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {

			if(id2[0] == temp_head->cBegin){
				
				ITEM6* pOldItem = (ITEM6*)temp_head->pItems;
				temp_head->pItems = (void*)item;
				ITEM6* pCurrentItem = (ITEM6*)temp_head->pItems;
				pCurrentItem->pNext = pOldItem;
				break;
				
			}
			else if (id2[0] > temp_head->cBegin && temp_head->pNext != 0) {
				pPrev = temp_head;
			}
			else if (id2[0] > temp_head->cBegin && temp_head->pNext == 0) {
				HEADER_A* my_head = new HEADER_A;
				my_head->cBegin = id2[0];
				my_head->pItems = (void*)item;
				my_head->pNext = 0;

				temp_head->pNext = my_head;
				break;
			}
			else if(id2[0] < temp_head->cBegin){

				HEADER_A* my_head = (HEADER_A*)malloc(sizeof(HEADER_A));
				my_head->cBegin = id2[0];
				my_head->pItems = (void*)item;

				if (pPrev != 0) {
					pPrev->pNext = my_head;
					my_head->pNext = temp_head;
				}
				else {
					HEADER_A* nextP = temp_head;
					ppStruct3[item_id[0] - 65] = my_head;
					my_head->pNext = nextP;
				}
				break;
			}
		}
	}
	else {
		HEADER_A* my_head = new HEADER_A;
		my_head->cBegin = id2[0];
		my_head->pNext = 0;
		my_head->pItems = (void*)item;
		ppStruct3[id1[0] - 65] = my_head;
	}
	
}

void DataStructure::operator-=(char* pItemID) {

	RemoveItem(ppStruct3, pItemID);
	
}

int DataStructure::operator==(DataStructure &Other){

	int item_num = GetItemsNumber(ppStruct3);
	int match = 0;

	for (int i = 0; i < 26; i++) {
		HEADER_A* temp_head = Other.ppStruct3[i];

		for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
			ITEM6* temp_item = (ITEM6*)temp_head->pItems;

			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {

				ITEM6* id_match = GetItem(temp_item->pID, ppStruct3);
				ITEM6* code_match = GetItem(temp_item->Code, ppStruct3);
				ITEM6* date_match = GetItem(temp_item->Date, ppStruct3);

				if (id_match != 0 && code_match != 0 && date_match != 0) {
					match++;
				}
			}
		}
	}

	if (match == item_num && GetItemsNumber(ppStruct3)==GetItemsNumber(Other.ppStruct3)) {
		return 1;
	}
	else {
		return 0;
	}

}

DataStructure &DataStructure::operator=(const DataStructure& Right) {

	//destructor
	for (int i = 0; i < 26; i++) {

		HEADER_A* pPrevHead = 0;

		if (ppStruct3[i] != 0) {

			HEADER_A* temp_head = ppStruct3[i];
			for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
				delete pPrevHead;

				ITEM6* pPrevItem = 0;
				ITEM6* temp_item = (ITEM6*)temp_head->pItems;
				for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {

					delete pPrevItem;
					delete (temp_item->pID);

					if (temp_item->pNext == 0) {
						delete temp_item;
						break;
					}
					else {
						pPrevItem = new ITEM6;
						pPrevItem = temp_item;
					}
				}

				if (temp_head->pNext == 0) {
					delete temp_head;
					break;
				}
				else {
					pPrevHead = new HEADER_A;
					pPrevHead = temp_head;
				}
			}
		}
	}


	for (int i = 0; i < 26; i++) {
		ppStruct3[i] = new HEADER_A;
		HEADER_A* right_head = Right.ppStruct3[i];
		HEADER_A* left_head = ppStruct3[i];

		if (right_head == 0) {
			ppStruct3[i] = 0;
		}
		else {

			for (right_head; right_head != 0; right_head = right_head->pNext) {
				left_head->cBegin = right_head->cBegin;

				left_head->pItems = (void*)malloc(sizeof(ITEM6));
				ITEM6* left_item = (ITEM6*)left_head->pItems;
				ITEM6* right_item = (ITEM6*)right_head->pItems;

				for (right_item; right_item != 0; right_item = right_item->pNext) {
					left_item->Code = right_item->Code;
					left_item->Date = right_item->Date;
					left_item->pID = new char[strlen(right_item->pID) + 1];
					strcpy(left_item->pID, right_item->pID);

					left_item->pNext = new ITEM6;
					if (right_item->pNext == 0) {
						left_item->pNext = 0;
					}
					else {
						left_item = left_item->pNext;
					}
				}

				left_head->pNext = new HEADER_A;
				if (right_head->pNext == 0) {
					left_head->pNext = 0;
				}
				else {
					left_head = left_head->pNext;
				}

			}
		}
	}

	return *this;
}

void DataStructure::Write(char* pFileName) {
	fstream File;
	File.open(pFileName, fstream::out | fstream::binary);

	for (int i = 0; i < 26; i++) {
		HEADER_A* temp_head = ppStruct3[i];
		for (temp_head; temp_head != 0; temp_head = temp_head->pNext) {
			File.write((char*)&temp_head->cBegin, sizeof(temp_head->cBegin));
			ITEM6* temp_item = (ITEM6*)temp_head->pItems;
			for (temp_item; temp_item != 0; temp_item = temp_item->pNext) {
				File.write((char*)&temp_item->Code, sizeof(unsigned long int));
				File.write((char*)&temp_item->Date.Day, sizeof(int));
				File.write((char*)&temp_item->Date.Month, sizeof(int));
				File.write((char*)&temp_item->Date.Year, sizeof(int));

				size_t len = strlen(temp_item->pID);
				File.write((char*)&len, sizeof(len));
				File.write(temp_item->pID, len);
			}
		}
	}
	
	File.close();
}


///////////////////////////////////////////////////////////////////////////


int main() {

	DataStructure A;

	ITEM6* my_item = (ITEM6*)GetItem(6);
	A += my_item;

	my_item = (ITEM6*)GetItem(6);
	A += my_item;

	my_item = (ITEM6*)GetItem(6);
	A += my_item;

	//A.Write((char*)"C:\\Users\\giorgi\\Desktop\\C.bin");

	//DataStructure B = (char*)"C:\\Users\\giorgi\\Desktop\\C.bin";



	DataStructure B = A;

	A -= my_item->pID;
	

	if (A == B) {
		printf("They are the same!\n");
	}
	else {
		printf("The number of items in A: %d\n", A.GetItemsNumber(A.ppStruct3));
		cout << A << endl;
		printf("The number of items in B: %d\n", B.GetItemsNumber(B.ppStruct3));
		cout << B << endl;
	}
	

	return 0;
}