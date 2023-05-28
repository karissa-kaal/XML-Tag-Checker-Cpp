#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//prototypes
template<class T>
class Node;
template<class T>
class Stack;
class XMLData;

template<class T>
class Node {
private:
	T Data;
	Node<T>* Next;
public:
	template <class T> friend class Stack;
	Node() { Data = 0; Next = NULL; }
	Node(T d, Node<T>* Ptr = NULL) { Data = d; Next = Ptr; }
};

template<class T>
class Stack {
private:
	Node<T>* Head;
public:
	Stack() { Head = NULL; }
	bool isEmpty();
	int size();
	bool top(T& TopData);
	void pop();
	void push(T const& e);
	~Stack();
};

class XMLData {
private:
	string StartTag;
	string EndTag;
	static int LineNo;
	static bool Header;
public:
	template<class T> friend void FileCheck(Stack<T>);
	XMLData() {
		StartTag = "";
		EndTag = "";
	}
	void ReadLine(ifstream&);
	bool CheckQuotations(ifstream&, char&);

	static int GetLine() { return LineNo; }
};

int XMLData::LineNo = 0;
bool XMLData::Header = false;

template<class T>
void FileCheck(Stack<T>);

//----------------------------------------------------------------------------------------------------------------

int main() {
	Stack<string> S1;

	FileCheck(S1);

	return 0;
}

//----------------------------------------------------------------------------------------------------------------

template<class T>
void FileCheck(Stack<T> S1) {
	ifstream Fin;
	string filename;
	cout << "Enter filename you want to check (Please enter in proper format e.g. \'file.txt\'):\t";
	cin >> filename;
	Fin.open(filename);

	if (!Fin.is_open()) {
		//checking if file is available
		cout << "\nFile not available\n";
		return;
	}


	while (!Fin.eof()) {
		//creating new obj for each line
		XMLData Obj;
		Obj.ReadLine(Fin);

		if (Obj.StartTag.compare("") && !Obj.EndTag.compare(""))//checking if start tag is there
			S1.push(Obj.StartTag);

		else if (!Obj.StartTag.compare("") && Obj.EndTag.compare("")) {
			//in case of end tag
			string temp;
			bool top = S1.top(temp); //taking top element

			if (!top) {
				//when no top tag
				cout << "\nError:\tSome change in tags. Start Tag to this tag not found. Line No:\t" << Obj.LineNo;
			}
			else {
				if (!Obj.EndTag.compare(temp))
					S1.pop();//pop when both are same
				else {
					cout << "\nError:\tEnd Tag Missing at Line no:\t" << Obj.LineNo;
					S1.pop();
				}
			}

		}
	}
	if (!S1.isEmpty())//when stack is not empty - some tag left unpoped
		cout << "\nClosing Tag is missing at line no:\t" << XMLData::GetLine();

	cout << "\nFile Checked\n";
}

//----------------------------------------------------------------------------------------------------------------
//Class Stack

template<class T>
bool Stack<T>::isEmpty() {
	//to check if stack is empty
	if (Head == NULL)
		return true;
	else
		return false;
}

template<class T>
int Stack<T>::size() {
	//giving size
	if (!isEmpty()) {
		int count = 0;
		Node<T>* Temp = Head;
		while (Temp != NULL) {
			count++;
			Temp = Temp->Next;
		}
	}
	return count;
}

template<class T>
bool Stack<T>::top(T& TopData) {
	//passing top element by reference
	if (isEmpty())
		return false;
	else
		TopData = Head->Data;
	return true;
}

template<class T>
void Stack<T>::pop() {
	//poping
	if (isEmpty())
		return;
	Node<T>* Temp = Head->Next;
	delete Head;
	Head = Temp;
}

template<class T>
void Stack<T>::push(T const& e) {
	//adding new
	Head = new Node<T>(e, Head);
}

template<class T>
Stack<T>::~Stack() {
	//destructor
	while (!isEmpty())
		pop();
}

//----------------------------------------------------------------------------------------------------------------
//Class XMLData

void XMLData::ReadLine(ifstream& fin) {
	if (fin.eof())
		return;

	char temp;			//for temprary storage

	LineNo++;
	do {
		if (fin.eof())
			return;
		temp = fin.get();
		if (temp == '<') {
			if (fin.peek() == '?' && LineNo == 1) {
				//xml header
				temp = fin.get();
				temp = fin.get();
				do {
					if (temp == '=')
						if (!CheckQuotations(fin, temp)) {
							cout << "\nError:\tQuotations missing in line no:\t" << LineNo;
						}
					if (temp != '?')
						temp = fin.get();
				} while (temp != '?' && temp != '\n');//reaching '?'

				if (temp == '?') {
					temp = fin.get();
					if (temp == '>')
						Header = true;//true when header is present
				}

				if (!Header) {
					cout << "\nError:\tHeader not in proper format. Line no:\t" << LineNo;
				}

			}

			else if (fin.peek() == '/') {
				//for end tags
				temp = fin.get();
				do {
					temp = fin.get();
					EndTag = EndTag + temp;
				} while (fin.peek() != '>' && fin.peek() != '\n' && !fin.eof());
				temp = fin.get();

				if (temp != '>') {
					cout << "\nError:\tMissing \'>\'. Line no:\t" << LineNo;
				}

			}

			else if (fin.peek() == '!') {
				//comment
				temp = fin.get();
				do {
					temp = fin.get();
				} while (fin.peek() != '>' && fin.peek() != '\n' && !fin.eof());
				temp = fin.get();

				if (temp != '>') {
					cout << "\nError:\tMissing \'>\'. Line no:\t" << LineNo;
				}
			}

			else {
				//for start tags
				do {
					temp = fin.get();
					StartTag = StartTag + temp;
				} while (fin.peek() != '>' && fin.peek() != ' ' && fin.peek() != '\n' && fin.peek() != '=');

				temp = fin.get();
				if (temp == ' ') {
					do {
						//for attribute
						temp = fin.get();
					} while (temp != '=');
				}

				if (temp == '=')
					if (!CheckQuotations(fin, temp)) {
						cout << "\nError:\tQuotations missing in line no:\t" << LineNo;
					}

					else if (temp == '\n') {
						cout << "\nError:\tMissing \'>\'. Line no:\t" << LineNo;
					}
			}
		}

		if (!StartTag.compare(EndTag) && StartTag.compare("") && EndTag.compare("")) {
			//when both are same but not ""
			StartTag = "";
			EndTag = "";
		}

		if (LineNo == 1 && !Header) {
			cout << "\nError:\tHeader not present";
			Header = true;
		}

	} while (temp != '\n');

}

bool XMLData::CheckQuotations(ifstream& fin, char& temp) {
	temp = fin.get();

	if (temp == ' ' || (temp != '\'' && temp != '\"')) {
		//removing all spaces and letters if present
		do {
			temp = fin.get();
		} while (temp != '\'' && temp != '\"');
	}

	if (temp == '\'') {
		//in case of single quotations
		do {
			temp = fin.get();
		} while (temp != '\'' && temp != '>' && temp != '\n' && temp != '?');

		if (temp != '\'')
			return false;
	}

	else if (temp == '\"') {
		//in case of double quotations
		do {
			temp = fin.get();
		} while (temp != '\"' && temp != '>' && temp != '\n' && temp != '?');

		if (temp != '\"')
			return false;
	}

	return true;
}