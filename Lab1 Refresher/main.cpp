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
	CORRECT_ID_LEN = ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE + ID_NUM_DIGITS_AT_END;

const string INPUT_FILE_NAME = "test.txt", INVALID_REC_FILE_NAME = "InvalidRecords.txt";

using namespace std;

enum Menu {
	PRINT_VALID = 1, PRINT_INVALID = 2, QUIT = 3
};

//Gets the data from the input file and writes all invalid data to error file
void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec);

//prints the valid records to the screen in table format
void PrintValidRecords(Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header); 

//prints the invalid records to the screen in table format
void PrintInvalidRecords(string border, string header); 

bool IsValidID(string carID, string& errorMessage); //checks if the Car ID is valid

bool IsValidModel(string model, string& errorMessage); //checks if the Model Name is valid

bool IsValidQuantity(int quantity, string& errorMessage); //checks if the quantity is valid

bool IsValidPrice(double price, string& errorMessage); //checks if the price is valid

string toUpper(string str); //returns a string with all letters upper case

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
	return 0;
}

void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec) {
	string tempCarID, tempModel, errMsg = "";
	int tempQuantityOnHand;
	double tempPrice;
	bool isValidID, isValidModel, isValidQuantity, isValidPrice;
	ifstream inFile(INPUT_FILE_NAME);
	ofstream errorFile(INVALID_REC_FILE_NAME);
	Record temp;
	if (!inFile) {
		cout << "Input file not found. Exiting the prgram." << endl;
		system("pause");
		exit(EXIT_FAILURE);

	}	
	//open Error file and if failed close input
	if (!errorFile) {
		cout << "Error file not found. Exiting the prgram." << endl;
		inFile.close();
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (inFile.peek() == EOF) { // use peek()
		cout << "The input file is empty. Quitting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
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
			errorFile << setw(SETW_ID) << left << tempCarID  << setw(SETW_MODEL) << tempModel << setw(SETW_QUANTITY) << right 
				<< tempQuantityOnHand  << setw(SETW_PRICE) << fixed << setprecision(2) << tempPrice << " " << errMsg << endl;
		}
	}

	if (numValidRec == MAX_NUM_RECORDS && !inFile.eof()) {
		cout << "Not all Records have been stored; only the first " << MAX_NUM_RECORDS << " were processed." << endl;
	}
	inFile.close();
	errorFile.close();
}

void PrintValidRecords(Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header) {
	if (numValidRec == 0) {
		cout << "NO VALID RECORDS FOUND.\n";
	}
	else {
		cout << "DISPLAYING VALID RECORDS...\n";
		cout << border << "\n" << header;
		for (int i = 0; i < numValidRec; i++) {
			cout << validRec[i].toString();
		}
		cout << border << "\n";
	}
}

void PrintInvalidRecords(string border, string header) {
	ifstream invalidRecFile(INVALID_REC_FILE_NAME);
	if (!invalidRecFile) {
		cout << "Invalid Records file not found. Exiting the prgram." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	//remove variables
	if (invalidRecFile.peek() == EOF) { // use peek()
		cout << "NO INVALID RECORDS FOUND\n";
	}
	else {
		cout << "DISPLAYING INVALID RECORDS...\n";
		cout << border << "\n" << header;
		string line;
		while (getline(invalidRecFile, line)) {
			cout << line << endl;
		}
		cout << border << "\n";
	}
}

bool IsValidID(string carID, string& errorMessage) {
	bool isIDCorrectLen = true, meetsIDCriteria1 = true, meetsIDCriteria2 = true, meetsIDCriteria3 = true; //make part1 part2 part3
	string idErrorMessage = "";
	if (carID.length() != CORRECT_ID_LEN) //const
	{
		isIDCorrectLen = false;
		//errorMessage += "The car ID must be " + to_string(CORRECT_ID_LEN) + " characters long. ";
		idErrorMessage += " Must be " + to_string(CORRECT_ID_LEN) + " characters long.";
	}
	else {
		//change err msg
		for (int i = 0; i < ID_NUM_OF_LEADING_CHAR_ALPHA && meetsIDCriteria1; i++) {
			char ch = carID[i];
			if (!(((ch >= 'A') && (ch <= 'Z'))) || ch == 'O' || ch == 'o') {
				meetsIDCriteria1 = false;
				/*errorMessage += "First " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA) + " characters of ID must be Alpha(A - Z, letter O is not allowed). ";*/
				idErrorMessage += " First " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA) + " characters must be Alpha(A - Z, letter O is not allowed).";
			}
		}

		for (int i = ID_NUM_OF_LEADING_CHAR_ALPHA; i < (ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE) && meetsIDCriteria2; i++) {
			char ch = carID[i];
			if (!((((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9'))) || ch == 'O') {
				meetsIDCriteria2 = false;
				/*errorMessage += "Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + 1) + 
					"-" + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE) +
					" of ID must be alphanumeric(A - Z, letter O is not allowed). ";*/
				idErrorMessage += " Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + 1) +
					"-" + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE) +
					" must be alphanumeric(A - Z, letter O is not allowed).";
			}
		}
		for (int i = (ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE); i < CORRECT_ID_LEN && meetsIDCriteria3; i++) {
			char ch = carID[i];
			if (!(ch >= '0' && ch <= '9')) {
				meetsIDCriteria3 = false;
				/*errorMessage += "Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE + 1) +
					"-" + to_string(CORRECT_ID_LEN) +
					" of ID must be numeric (0-9).";*/
				idErrorMessage += " Characters " + to_string(ID_NUM_OF_LEADING_CHAR_ALPHA + ID_NUM_OF_ALPHANUM_IN_MIDDLE + 1) +
					"-" + to_string(CORRECT_ID_LEN) +
					" must be numeric (0-9).";
			}
		}
	}

	if (!(isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3)) {
		errorMessage += "INVALID ID [" + idErrorMessage + "] ";  
	}
	return isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3;
}

bool IsValidModel(string model, string& errorMessage) {
	bool isModelMinLen = true, meetsModelCriteria1 = true, meetsModelCriteria2 = true; //change variables
	string modelErrorMessage = "";
	if (model.length() < MIN_MODEL_LEN) {
		isModelMinLen = false;
		//errorMessage += "The model name must be at least " + to_string(MIN_MODEL_LEN) + " characters long. ";
		modelErrorMessage +=  "Must be at least " + to_string(MIN_MODEL_LEN) + " characters long.";
	}
	if (model[0] < 'A' || model[0] > 'Z') {
		meetsModelCriteria1 = false;
		//errorMessage += "The name must start with a letter. ";
		modelErrorMessage += " Must start with a letter.";
	}
	for (int i = 1; i < model.length() && meetsModelCriteria2; i++) {
		char ch = model[i];
		if (!((((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9')))) {
			meetsModelCriteria2 = false;
			//errorMessage += "The name must consist of only alphanumeric characters.";
			modelErrorMessage += " Must consist of only alphanumeric characters.";
		}
	}
	if (!(isModelMinLen && meetsModelCriteria1 && meetsModelCriteria2)) {
		errorMessage += "INVALID MODEL [" + modelErrorMessage + "] ";
	}

	return isModelMinLen && meetsModelCriteria1 && meetsModelCriteria2;
}

bool IsValidQuantity(int quantity, string& errorMessage) {
	if (quantity < MIN_QUANTITY) {
		errorMessage += "INVALID QUANTITY [Quantity must be greater than or equal to " + to_string(MIN_QUANTITY) + ".] ";
	}
	
	return quantity >= MIN_QUANTITY;
}

bool IsValidPrice(double price, string& errorMessage) {
	if (price <= MIN_PRICE) {
		errorMessage += "INVALID PRICE [Price must be greater than $" + to_string(MIN_PRICE) + ".] ";
	}
	return price > MIN_PRICE;
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

//TEST RUNS

// TEST #1 (Some Valid and Invalid Data)
/*
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
dsafdsfs
INVALID MENU SELECTION
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
1
DISPLAYING VALID RECORDS...
--------------------------------------------------
Car ID         Model        Quantity         Price

LP8E5V159      CORVETTE            0      70540.81
DSN0DG715      OUTBACK            75      26000.73
ALY13M398      PACIFICA           90      28000.11
LAX9NN745      MODEL3             30      48200.00
KDTYGA123      COROLLA             0      21500.99
AS2QQB593      CIVIC              20      15230.24
BGYZUV178      MUSTANG            14      50090.54
MK1Q7U364      GT6                 1     134520.73
TEZNID268      ALTIMA            120      25340.99
PQXGXC756      SONATA             36      19990.12
--------------------------------------------------
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
2
DISPLAYING INVALID RECORDS...
--------------------------------------------------
Car ID         Model        Quantity         Price

OANKGA123      CAMRY               0      21500.99 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).]
A1APP5122      CIVIC              20      15230.24 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).]
BG[ZUVABC      MUSTANG            14      50090.54 INVALID ID [ Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed). Characters 7-9 must be numeric (0-9).]
MK1O7836       GT6                 1     134520.73 INVALID ID [ Must be 9 characters long.]
LP8E\V1599     CORVETTE            9      70540.81 INVALID ID [ Must be 9 characters long.]
1EZNI;268      AL4               120      25340.99 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed). Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed).]
PQXGXCA56      SONATA            -36      19990.12 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID QUANTITY [Quantity must be greater than or equal to 0.]
DSN0DG7P5      OUTBACK            75     -26000.73 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID PRICE [Price must be greater than $5000.]
LLY13M39L      PACIFICA           90      28000.11 INVALID ID [ Characters 7-9 must be numeric (0-9).]
CAX9UN74       MODEL3             30      48200.00 INVALID ID [ Must be 9 characters long.]
ABÆ[SDF1234    LXE                 1       5000.01 INVALID ID [ Must be 9 characters long.]
PDNKGA123      4CAMRY              0      21500.99 INVALID MODEL [ Must start with a letter.]
AS2QQB593      CV                 20      15230.24 INVALID MODEL [Must be at least 3 characters long.]
BGYZUV178      I8                 14      50090.54 INVALID MODEL [Must be at least 3 characters long.]
MK1Q7U364      GT6                -1     134520.73 INVALID QUANTITY [Quantity must be greater than or equal to 0.]
TEZNID268      ALTIMA            120     -25340.99 INVALID PRICE [Price must be greater than $5000.]
PQXGXC756      SONATA             36       4999.99 INVALID PRICE [Price must be greater than $5000.]
--------------------------------------------------
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
3
Exiting out of the program...
*/

//TEST #2 (All invalid records)
/*
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
1
NO VALID RECORDS FOUND.
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
3
Exiting out of the program...

*/

//TEST #3 (All Valid Records)
/*
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
2
NO INVALID RECORDS FOUND
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
3
Exiting out of the program...

*/

//TEST #4 (Input File is empty)
/*
The input file is empty. Quitting the program.
Press any key to continue . . .
*/