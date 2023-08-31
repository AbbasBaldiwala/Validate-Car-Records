#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Record.h"

const int MAX_NUM_RECORDS = 30, CORRECT_ID_LEN = 9, MIN_MODEL_LEN = 3, SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14;;
//int SETW_NUM = 15;
const string INPUT_FILE_NAME = "test.txt", INVALID_REC_FILE_NAME = "InvalidRecords.txt";

using namespace std;

enum Menu {
	PRINT_VALID = 1, PRINT_INVALID = 2, QUIT = 3
};

int GetData(Record validRec[MAX_NUM_RECORDS]);

void WriteInvalidRecords(string carID, string model, int quantity, double price, ofstream& outFile, string errMsg);

void PrintInvalidRecords();

bool IsValidID(string carID, string& errorMessage);

bool IsValidModel(string model, string& errorMessage);

bool IsValidQuantity(int quantity, string& errorMessage);

bool IsValidPrice(double price, string& errorMessage);

int main() {
	int userChoice, numValidRec;
	bool userWantsToQuit = false;
	stringstream border, header;
	Record validRecords[MAX_NUM_RECORDS];
	cout << "Choose one of the following options: \n";
	cout << "1. Print valid records\n";
	cout << "2. Print Invalid Records\n";
	cout << "3. Quit the program\n";
	cin >> userChoice;
	numValidRec = GetData(validRecords);

	for (int i = 0; i < (SETW_ID + SETW_MODEL + SETW_PRICE + SETW_QUANTITY); i++) {
		border << "-";
	}

	header << setw(SETW_ID) << left << "Car ID" << setw(SETW_MODEL) << "Model" << setw(SETW_QUANTITY) << right << "Quantity" << setw(SETW_PRICE + 1) << right << "Price\n" << endl;

	while (!userWantsToQuit) {
		switch (userChoice) {
		case PRINT_VALID:
			cout << "DISPLAYING VALID RECORDS...\n";
			cout << border.str() << "\n";
			cout << header.str();
			for (int i = 0; i < numValidRec; i++) {
				cout << validRecords[i].toString();
			}
			cout << border.str() << "\n";
			break;
		case PRINT_INVALID:
			cout << "DISPLAYING INVALID RECORDS...\n";
			cout << border.str() << "\n";
			cout << header.str();
			PrintInvalidRecords();
			cout << border.str() << "\n";
			break;
		case QUIT:
			userWantsToQuit = true;
			cout << "Exiting out of the program...\n";
			break;
		default:
			cout << "You need to enter a number between 1 and 3!\n";
		}
		if (!userWantsToQuit) {
			cout << "Choose one of the following options: \n";
			cout << "1. Print valid records\n";
			cout << "2. Print Invalid Records\n";
			cout << "3. Quit the program\n";
			cin >> userChoice;
		}
	}
}

int GetData(Record validRec[MAX_NUM_RECORDS]) {
	int numValidRec = 0, numInvalidRec = 0;
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
	while (!inFile.eof() && numValidRec < MAX_NUM_RECORDS) {
		inFile >> tempCarID >> tempModel >> tempQuantityOnHand >> tempPrice;
		
		isValidID = IsValidID(tempCarID, errMsg);
		isValidModel = IsValidModel(tempModel, errMsg);
		isValidQuantity = IsValidQuantity(tempQuantityOnHand, errMsg);
		isValidPrice = IsValidPrice(tempPrice, errMsg);
		if (isValidID && isValidModel && isValidQuantity && isValidPrice) {
			temp.SetRecord(tempCarID, tempModel, tempQuantityOnHand, tempPrice);
			temp.toUpper();
			validRec[numValidRec] = temp;
			numValidRec++;
		}
		else {
			numInvalidRec++;
			if (outFile.is_open()) {
				WriteInvalidRecords(tempCarID, tempModel, tempQuantityOnHand, tempPrice, outFile, errMsg);
			}
		}
	}
	if (numValidRec == 0 && numInvalidRec == 0) {
		cout << "The input file is empty. Quitting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	else if (numValidRec == MAX_NUM_RECORDS && !inFile.eof()) {
		cout << "Not all Records have been stored; only the first " << MAX_NUM_RECORDS << " were processed." << endl;
	}
	inFile.close();
	outFile.close();
	return numValidRec;
}

void WriteInvalidRecords(string carID, string model, int quantity, double price, ofstream& outFile, string errMsg) {
	outFile << carID << " " << model << " " << quantity << " " << price << " ";
	outFile << errMsg << endl;
}

void PrintInvalidRecords() {
	ifstream invalidRecFile(INVALID_REC_FILE_NAME);
	string id, model, errMsg;
	int quantity;
	double price;
	if (!invalidRecFile) {
		cout << "Invalid Records file not found. Exiting the prgram." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	invalidRecFile >> id >> model >> quantity >> price;
	getline(invalidRecFile, errMsg);
	while (!invalidRecFile.eof()) {
		cout << setw(SETW_ID) << left << id << setw(SETW_MODEL) << model << setw(SETW_QUANTITY) << right << quantity << setw(SETW_PRICE) << fixed << setprecision(2) << price << errMsg << endl;
		invalidRecFile >> id >> model >> quantity >> price;
		getline(invalidRecFile, errMsg);
	}
}

bool IsValidID(string carID, string& errorMessage) {
	bool is9CharLong = true, first2Alpha = true, next4AlphaNum = true, last3Num = true;
	if (carID.length() != 9)
	{
		is9CharLong = false;

	}
	else {
		for (int i = 0; i < 2; i++) {
			char ch = carID[i];
			if (!(((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z'))) || ch == 'O' || ch == 'o') {
				first2Alpha = false;
			}
		}

		for (int i = 2; i < 6; i++) {
			char ch = carID[i];
			if (!(((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) || ch == 'O' || ch == 'o') {
				next4AlphaNum = false;
			}
		}
		for (int i = 6; i < 9; i++) {
			char ch = carID[i];
			if (!(ch >= '0' && ch <= '9')) {
				last3Num = false;

			}
		}
	}
	errorMessage = "";

	if (!(is9CharLong && first2Alpha && next4AlphaNum && last3Num)) {
		errorMessage += " Invalid ID [";
		if (!is9CharLong) {
			errorMessage += "The car ID must be 9 characters long ";
		}
		if (!first2Alpha) {
			errorMessage += "The first 2 characters must be Alpha (A-Z, letter O is not allowed) ";
		}
		if (!next4AlphaNum) {
			errorMessage += "Characters 3-6 must be alphanumeric (A-Z, letter O is not allowed) ";
		}
		if (!last3Num) {
			errorMessage += " Characters 7-9 characters must be numeric (0-9).";
		}
		errorMessage += "]";
	}
	return is9CharLong && first2Alpha && next4AlphaNum && last3Num;
}

bool IsValidModel(string model, string& errorMessage) {
	bool isMin3CharLong = true, startsWithAlpha = true, isAlphaNum = true;
	if (model.length() < 3) {
		isMin3CharLong = false;
	}
	if (model[0] < 'A' || model[0] > 'Z') {
		startsWithAlpha = false;
	}
	for (int i = 1; i < model.length(); i++) {
		char ch = model[i];
		if (!(((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')))) {
			isAlphaNum = false;
		}
	}
	if (!(isMin3CharLong && startsWithAlpha && isAlphaNum)) {
		errorMessage += " Invalid Model [";
		if (!isMin3CharLong) {
			errorMessage += " The model name must be at least 3 characters long.";
		}
		if (!startsWithAlpha) {
			errorMessage += " The name must start with a capital letter.";
		}
		if (!isAlphaNum) {
			errorMessage += " The name must consist of only alphanumeric characters.";
		}
		errorMessage += "]";
	}

	return isMin3CharLong && startsWithAlpha && isAlphaNum;
}

bool IsValidQuantity(int quantity, string& errorMessage) {
	if (quantity < 0) {
		errorMessage += " Invalid Quantity [The quantity must be greater than or equal to 0]";
	}
	
	return quantity >= 0;
}

bool IsValidPrice(double price, string& errorMessage) {
	if (price <= 5000.00) {
		errorMessage += " Invalid Price [The Price must be greater than $5000.00]";
	}
	return price > 5000.00;
}