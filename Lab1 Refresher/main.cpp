#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include "CarRecord.h"

const int MAX_NUM_RECORDS = 30, MIN_MODEL_LEN = 3,
	SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14,
	TABLE_LENGTH = SETW_ID + SETW_MODEL + SETW_PRICE + SETW_QUANTITY,
	MIN_PRICE = 5000, MIN_QUANTITY = 0, ASCII_OFFSET = 32,
	//ID Criteria
	ID_NUM_OF_LEADING_CHAR_ALPHA = 2, ID_NUM_OF_ALPHANUM_IN_MIDDLE = 4, ID_NUM_DIGITS_AT_END = 3,
	CORRECT_ID_LEN = ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE + ID_NUM_DIGITS_AT_END;//add more const

const string INPUT_FILE_NAME = "test.txt", INVALID_REC_FILE_NAME = "InvalidRecords.txt";

using namespace std;

//Menu
enum Menu {
	PRINT_VALID = 1, PRINT_INVALID = 2, QUIT = 3
};

//Gets the data from the input file
void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec);

void PrintValidRecords(Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header);

void PrintInvalidRecords(string border, string header); //prints the invalid records to the screen

//checks if the Car ID is valid
bool IsValidID(string carID, string& errorMessage);

//Checks if the Model Name is valid
bool IsValidModel(string model, string& errorMessage);

//Checks if the Quantity is valid
bool IsValidQuantity(int quantity, string& errorMessage);

//Checks if the Price is valid
bool IsValidPrice(double price, string& errorMessage);

//remove below
//Checks if the character is a letter
bool isLetter(char ch);

//checks if the character is a number
bool isNumber(char ch);

string toUpper(string str);

int main() {
	int userChoice, numValidRec = 0;
	stringstream border, header;
	Record validRecords[MAX_NUM_RECORDS];
	
	GetData(validRecords, numValidRec);
	
	border << setfill('-') << setw(TABLE_LENGTH) << "";
	header << setw(SETW_ID) << left << "Car ID" << setw(SETW_MODEL) << "Model" << setw(SETW_QUANTITY) << right << "Quantity" << setw(SETW_PRICE + 1) << right << "Price\n" << endl;


		
	do {
		cout << "Choose one of the following options: \n"
			"1. Print valid records\n"
			"2. Print Invalid Records\n"
			"3. Quit the program\n";
		cin >> userChoice;
		switch (userChoice) {
		case PRINT_VALID:
			PrintValidRecords(validRecords, numValidRec, border.str(), header.str());
			break;
		case PRINT_INVALID:
			PrintInvalidRecords(border.str(), header.str());
			break;
		case QUIT:
			cout << "Exiting out of the program...\n";
			break;
		default:
			cout << "INVALID MENU SELECTION\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (userChoice != QUIT);
}

void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec) {
	int numInvalidRec = 0;
	string tempCarID, tempModel, errMsg = "";
	int tempQuantityOnHand;
	double tempPrice;
	bool isValidID, isValidModel, isValidQuantity, isValidPrice;
	ifstream inFile(INPUT_FILE_NAME);
	ofstream outFile(INVALID_REC_FILE_NAME);
	Record temp;
	if (!inFile) {
		cout << "Input file not found. Exiting the prgram." << endl;
		system("pause");
		exit(EXIT_FAILURE);

	}
	//open output file and if failed close input
	while (!inFile.eof() && numValidRec < MAX_NUM_RECORDS) {
		inFile >> tempCarID >> tempModel >> tempQuantityOnHand >> tempPrice;
		//capigalize car id and model
		tempCarID = toUpper(tempCarID);
		tempModel = toUpper(tempModel);
		errMsg = "";
		isValidID = IsValidID(tempCarID, errMsg);
		isValidModel = IsValidModel(tempModel, errMsg);
		isValidQuantity = IsValidQuantity(tempQuantityOnHand, errMsg);
		isValidPrice = IsValidPrice(tempPrice, errMsg);
		if (isValidID && isValidModel && isValidQuantity && isValidPrice) {
			validRec[numValidRec].SetRecord(tempCarID, tempModel, tempQuantityOnHand, tempPrice);
			numValidRec++;
		}
		else {
			numInvalidRec++;
			outFile << tempCarID << " " << tempModel << " " << tempQuantityOnHand << " " << tempPrice << " " << errMsg << endl;
		}
	}
	if (numValidRec == 0 && numInvalidRec == 0) { // use peek()
		cout << "The input file is empty. Quitting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	else if (numValidRec == MAX_NUM_RECORDS && !inFile.eof()) {
		cout << "Not all Records have been stored; only the first " << MAX_NUM_RECORDS << " were processed." << endl;
	}
	inFile.close();
	outFile.close();
}

void PrintValidRecords(Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header) {
	cout << "DISPLAYING VALID RECORDS...\n";
	cout << border << "\n" << header;
	for (int i = 0; i < numValidRec; i++) {
		cout << validRec[i].toString();
	}
	cout << border << "\n";
}
void PrintInvalidRecords(string border, string header) {
	ifstream invalidRecFile(INVALID_REC_FILE_NAME);
	string id, model, errMsg;
	int quantity;
	double price;
	if (!invalidRecFile) {
		cout << "Invalid Records file not found. Exiting the prgram." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	//remove variables
	invalidRecFile >> id >> model >> quantity >> price;
	getline(invalidRecFile, errMsg);
	cout << "DISPLAYING INVALID RECORDS...\n";
	cout << border << "\n" << header;
	//dont store just output
	while (!invalidRecFile.eof()) {
		cout << setw(SETW_ID) << left << id << setw(SETW_MODEL) << model << setw(SETW_QUANTITY) << right << quantity << setw(SETW_PRICE) << fixed << setprecision(2) << price << errMsg << endl;
		invalidRecFile >> id >> model >> quantity >> price;
		getline(invalidRecFile, errMsg);
	}
	cout << border << "\n";
}

bool IsValidID(string carID, string& errorMessage) {
	bool isIDCorrectLen = true, meetsIDCriteria1 = true, meetsIDCriteria2 = true, meetsIDCriteria3 = true; //make part1 part2 part3
	if (carID.length() != CORRECT_ID_LEN) //const
	{
		isIDCorrectLen = false;
		errorMessage += "The car ID must be " + to_string(CORRECT_ID_LEN) + " characters long. ";
	}
	else {
		//change err msg
		for (int i = 0; i < ID_NUM_OF_LEADING_CHAR_ALPHA && meetsIDCriteria1; i++) {
			char ch = carID[i];
			if (!isLetter(ch) || ch == 'O' || ch == 'o') {
				meetsIDCriteria1 = false;
				errorMessage += "First " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA) + " characters of ID must be Alpha(A - Z, letter O is not allowed). ";
			}
		}

		for (int i = ID_NUM_OF_LEADING_CHAR_ALPHA; i < (ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE) && meetsIDCriteria2; i++) {
			char ch = carID[i];
			if (!(isLetter(ch) || isNumber(ch)) || ch == 'O' || ch == 'o') {
				meetsIDCriteria2 = false;
				errorMessage += "Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + 1) + 
					"-" + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE) +
					" of ID must be alphanumeric(A - Z, letter O is not allowed). ";
			}
		}
		for (int i = (ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE); i < CORRECT_ID_LEN && meetsIDCriteria3; i++) {
			char ch = carID[i];
			if (!(ch >= '0' && ch <= '9')) {
				meetsIDCriteria3 = false;
				errorMessage += "Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE + 1) +
					"-" + to_string(CORRECT_ID_LEN) +
					" of ID must be numeric (0-9).";
			}
		}
	}

	//if (!(isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3)) {
	//	errorMessage += " Invalid ID [";
	//	if (!isIDCorrectLen) {
	//		errorMessage += "The car ID must be 9 characters long. ";
	//	}
	//	if (!meetsIDCriteria1) {
	//		//errorMessage += "The first 2 characters of ID must be Alpha (A-Z, letter O is not allowed). ";
	//	}
	//	if (!meetsIDCriteria2) {
	//		//errorMessage += "Characters 3-6 must  of ID be alphanumeric (A-Z, letter O is not allowed). ";
	//	}
	//	if (!meetsIDCriteria3) {
	//		//errorMessage += "Characters 7-9 characters must be numeric (0-9).";
	//	}
	//	errorMessage += "]";
	//}
	return isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3;
}

bool IsValidModel(string model, string& errorMessage) {
	bool isModelMinLen = true, meetsModelCriteria1 = true, meetsModelCriteria2 = true; //change variables
	if (model.length() < MIN_MODEL_LEN) {
		isModelMinLen = false;
		errorMessage += "The model name must be at least " + to_string(MIN_MODEL_LEN) + " characters long. ";
	}
	if (model[0] < 'A' || model[0] > 'Z') {
		meetsModelCriteria1 = false;
		errorMessage += "The name must start with a letter. ";
	}
	for (int i = 1; i < model.length() && meetsModelCriteria2; i++) {
		char ch = model[i];
		if (!(isLetter(ch) || isNumber(ch))) {
			meetsModelCriteria2 = false;
			errorMessage += "The name must consist of only alphanumeric characters.";
		}
	}
	//if (!(isModelMinLen && meetsModelCriteria1 && meetsModelCriteria2)) {
	//	errorMessage += " Invalid Model [";
	//	if (!isModelMinLen) {
	//		//errorMessage += "The model name must be at least 3 characters long. ";
	//	}
	//	if (!meetsModelCriteria1) {
	//		//errorMessage += "The name must start with a letter. ";
	//	}
	//	if (!meetsModelCriteria2) {
	//		//errorMessage += "The name must consist of only alphanumeric characters.";
	//	}
	//	errorMessage += "]";
	//}

	return isModelMinLen && meetsModelCriteria1 && meetsModelCriteria2;
}

bool IsValidQuantity(int quantity, string& errorMessage) {
	if (quantity < MIN_QUANTITY) {
		errorMessage += "The quantity must be greater than or equal to " + to_string(MIN_QUANTITY) + ". ";
	}
	
	return quantity >= MIN_QUANTITY;
}

bool IsValidPrice(double price, string& errorMessage) {
	if (price <= MIN_PRICE) {
		errorMessage += "The price must be greater than $" + to_string(MIN_PRICE) + ".";
	}
	return price > MIN_PRICE;
}

bool isLetter(char ch) {
	return (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')));
}

bool isNumber(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}


string toUpper(string str) {
	string upperCaseStr = "";
	for (int i = 0; i < str.length(); i++) {
		char ch = str[i];
		// change no 26
		if (((ch >= 'a') && (ch <= 'z'))) {
			ch -= ASCII_OFFSET;
		}
		upperCaseStr += ch;
	}
	str = upperCaseStr;
	return upperCaseStr;
}