#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include "CarRecord.h"

using namespace std;
const int MAX_NUM_RECORDS = 30, 
	SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14,
	TABLE_LENGTH = SETW_ID + SETW_MODEL + SETW_PRICE + SETW_QUANTITY,
	MIN_PRICE = 5000, MIN_QUANTITY = 0, MIN_MODEL_LEN = 3,
	ID_CRIT_1_NUM_LETTERS = 2, ID_CRIT_2_NUM_ALPHANUMERIC = 4, ID_CRIT_3_NUM_DIGITS = 3,
	CORRECT_ID_LEN = ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC + ID_CRIT_3_NUM_DIGITS,
	ASCII_OFFSET = 32;

enum Menu {
	PRINT_VALID = 1, PRINT_INVALID = 2, QUIT = 3
};

//Gets the data from the input file and writes all invalid data to error file
void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec, string errorFileName);

//prints the valid records to the screen in table format
void PrintValidRecords(const Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header); 

//prints the invalid records to the screen in table format
void PrintInvalidRecords(string border, string header, string errorFileName);

bool IsValidID(string carID, string& errorMessage); //checks if the Car ID is valid

bool IsValidModel(string model, string& errorMessage); //checks if the Model Name is valid

bool IsValidQuantity(int quantity, string& errorMessage); //checks if the quantity is valid

bool IsValidPrice(double price, string& errorMessage); //checks if the price is valid

string toUpper(string str); //returns string converted to upper case

int main() {
	int userChoice, numValidRec = 0;
	stringstream border, header;
	Record validRecords[MAX_NUM_RECORDS];
	string errorFileName = "InvalidRecords.txt";
	GetData(validRecords, numValidRec, errorFileName);
	
	border << setfill('-') << setw(TABLE_LENGTH) << "";
	header << setw(SETW_ID) << left << "Car ID" << setw(SETW_MODEL) << "Model" << setw(SETW_QUANTITY) << right << "Quantity" << setw(SETW_PRICE + 1) << right << "Price\n" << endl;
	
	do {
		cout << "Choose one of the following options: \n"
			"1. Print valid records\n"
			"2. Print Invalid Records\n"
			"3. Quit the program" << endl;
		cin >> userChoice;
		switch (userChoice) {
		case PRINT_VALID:
			PrintValidRecords(validRecords, numValidRec, border.str(), header.str());
			break;
		case PRINT_INVALID:
			PrintInvalidRecords(border.str(), header.str(), errorFileName);
			break;
		case QUIT:
			cout << "Exiting out of the program..." << endl;
			break;
		default:
			cout << "INVALID MENU SELECTION" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (userChoice != QUIT);
	return 0;
}

void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec, string errorFileName) {
	string tempCarID, tempModel, errMsg = "", inputFileName = "test.txt";
	int tempQuantityOnHand;
	double tempPrice;
	bool isValidID, isValidModel, isValidQuantity, isValidPrice;
	ifstream inFile(inputFileName);
	ofstream errorFile(errorFileName);
	Record temp;
	if (!inFile) {
		cout << "Input file not found. Exiting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}	
	if (!errorFile) {
		cout << "Error file not found. Exiting the prgram." << endl;
		inFile.close();
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (inFile.peek() == EOF) {
		cout << "The input file is empty. Quitting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	while (!inFile.eof() && numValidRec < MAX_NUM_RECORDS) { 
		inFile >> tempCarID >> tempModel >> tempQuantityOnHand >> tempPrice;

		tempCarID = toUpper(tempCarID);
		tempModel = toUpper(tempModel);

		errMsg = "";
		isValidID = IsValidID(tempCarID, errMsg);
		isValidModel = IsValidModel(tempModel, errMsg);
		isValidQuantity = IsValidQuantity(tempQuantityOnHand, errMsg);
		isValidPrice = IsValidPrice(tempPrice, errMsg);

		if (isValidID && isValidModel && isValidQuantity && isValidPrice) { //separates valid and invalid records
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

void PrintValidRecords(const Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header) {
	if (numValidRec == 0) {
		cout << "NO VALID RECORDS FOUND.\n";
	}
	else {
		cout << "DISPLAYING VALID RECORDS (UNSORTED)...\n";
		cout << border << "\n" << header;
		for (int i = 0; i < numValidRec; i++) {
			cout << validRec[i].toString();
		}
		cout << border << "\n";
	}
}

void PrintInvalidRecords(string border, string header, string errorFileName) {
	ifstream invalidRecFile(errorFileName);
	if (!invalidRecFile) {
		cout << "Invalid Records file not found. Exiting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (invalidRecFile.peek() == EOF) { 
		cout << "NO INVALID RECORDS FOUND\n";
	}
	else {
		cout << "DISPLAYING INVALID RECORDS...\n" 
			<< border << "\n" 
			<< header;
		string line;
		while (getline(invalidRecFile, line)) {
			cout << line << endl;
		}
		cout << border << endl;
	}
	invalidRecFile.close();
}

bool IsValidID(string carID, string& errorMessage) {
	bool isIDCorrectLen = true, meetsIDCriteria1 = true, meetsIDCriteria2 = true, meetsIDCriteria3 = true;
	string idErrorMessage = "";
	if (carID.length() != CORRECT_ID_LEN)
	{
		isIDCorrectLen = false;
		idErrorMessage += " Must be " + to_string(CORRECT_ID_LEN) + " characters long.";
	}
	else {
		for (int i = 0; i < ID_CRIT_1_NUM_LETTERS; i++) {
			char ch = carID[i];
			if (!(((ch >= 'A') && (ch <= 'Z'))) || ch == 'O' || ch == 'o') { //checks if ID meets criteria 1
				meetsIDCriteria1 = false;
				idErrorMessage += " First " + to_string(ID_CRIT_1_NUM_LETTERS) + " characters must be Alpha(A - Z, letter O is not allowed).";
				i = ID_CRIT_1_NUM_LETTERS;
			}
		}
		for (int i = ID_CRIT_1_NUM_LETTERS; i < (ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC); i++) { //checks if ID meets criteria 2
			char ch = carID[i];
			if (!((((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9'))) || ch == 'O') {
				meetsIDCriteria2 = false;
				idErrorMessage += " Characters " + to_string(ID_CRIT_1_NUM_LETTERS + 1) +
					"-" + to_string(ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC) +
					" must be alphanumeric(A - Z, letter O is not allowed).";
				i = ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC;
			}
		}
		for (int i = (ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC); i < CORRECT_ID_LEN; i++) { //checks if ID meets criteria 3
			char ch = carID[i];
			if (!(ch >= '0' && ch <= '9')) {
				meetsIDCriteria3 = false;
				idErrorMessage += " Characters " + to_string(ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC + 1) +
					"-" + to_string(CORRECT_ID_LEN) +
					" must be numeric (0-9).";
				i = CORRECT_ID_LEN;
			}
		}
	}

	if (!(isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3)) {
		errorMessage += "INVALID ID [" + idErrorMessage + "] ";  
	}
	return isIDCorrectLen && meetsIDCriteria1 && meetsIDCriteria2 && meetsIDCriteria3;
}

bool IsValidModel(string model, string& errorMessage) {
	bool isModelMinLen = true, meetsModelCriteria1 = true, meetsModelCriteria2 = true;
	string modelErrorMessage = "";
	if (model.length() < MIN_MODEL_LEN) {
		isModelMinLen = false;
		modelErrorMessage +=  "Must be at least " + to_string(MIN_MODEL_LEN) + " characters long."; 
	}
	if (model[0] < 'A' || model[0] > 'Z') { //checks if Model meets criteria 1
		meetsModelCriteria1 = false;
		modelErrorMessage += " Must start with a letter.";
	}
	int len = model.length();
	for (int i = 1; i < len; i++) { //checks if Model meets criteria 2
		char ch = model[i];
		if (!((((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9')))) {
			meetsModelCriteria2 = false;
			modelErrorMessage += " Must consist of only alphanumeric characters.";
			i = len;
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
		if (((ch >= 'a') && (ch <= 'z'))) {
			ch -= ASCII_OFFSET; //converts char to uppercase
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
jfdahsk
INVALID MENU SELECTION
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
1
DISPLAYING VALID RECORDS (UNSORTED)...
--------------------------------------------------
Car ID         Model        Quantity         Price

LP8E5V159      CAMERO              0      72380.81
DSN0DG715      CROSSOVER          75      26700.73
ALY13M398      BEATLE             90      19750.11
LAX9NN745      MODELX             30      41490.00
KDTYGA123      COROLLA6            0      22490.99
PLT6HJ764      LAFERRARI          20      16780.24
JGR5YG258      PRIUS              15      20090.54
HTG6T5679      LOTUS4              2     134535.73
IAT67T964      SENTRA            110      25840.99
KGYIG6765      FUSION5            22      19975.12
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
ABÆ[SDF1234    L;X                 1       5000.01 INVALID ID [ Must be 9 characters long.] INVALID MODEL [ Must consist of only alphanumeric characters.]
PDNKGA123      4CAMRY              0      21820.99 INVALID MODEL [ Must start with a letter.]
AS2QQB593      CV                 20      12340.24 INVALID MODEL [Must be at least 3 characters long.]
BGYZUV178      I8                 14      59990.54 INVALID MODEL [Must be at least 3 characters long.]
MK1Q7U364      GT7                -3     127420.73 INVALID QUANTITY [Quantity must be greater than or equal to 0.]
TEZNID268      ALTIMA            200     -20940.99 INVALID PRICE [Price must be greater than $5000.]
PQXGXC756      HELLCAT            36       4999.99 INVALID PRICE [Price must be greater than $5000.]
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

//TEST #5 (More valid records than array could store)
/*
Not all Records have been stored; only the first 8 were processed.
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
1
DISPLAYING VALID RECORDS (UNSORTED)...
--------------------------------------------------
Car ID         Model        Quantity         Price

LP8E5V159      CAMERO              0      72380.81
DSN0DG715      CROSSOVER          75      26700.73
ALY13M398      BEATLE             90      19750.11
LAX9NN745      MODELX             30      41490.00
KDTYGA123      COROLLA6            0      22490.99
PLT6HJ764      LAFERRARI          20      16780.24
JGR5YG258      PRIUS              15      20090.54
HTG6T5679      LOTUS4              2     134535.73
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
ABÆ[SDF1234    LX                  1       5000.01 INVALID ID [ Must be 9 characters long.] INVALID MODEL [Must be at least 3 characters long.]
PDNKGA123      4CAMRY              0      21820.99 INVALID MODEL [ Must start with a letter.]
AS2QQB593      CV                 20      12340.24 INVALID MODEL [Must be at least 3 characters long.]
BGYZUV178      I8                 14      59990.54 INVALID MODEL [Must be at least 3 characters long.]
MK1Q7U364      GT7                -3     127420.73 INVALID QUANTITY [Quantity must be greater than or equal to 0.]
TEZNID268      ALTIMA            200     -20940.99 INVALID PRICE [Price must be greater than $5000.]
PQXGXC756      HELLCAT            36       4999.99 INVALID PRICE [Price must be greater than $5000.]
--------------------------------------------------
Choose one of the following options:
1. Print valid records
2. Print Invalid Records
3. Quit the program
3
Exiting out of the program...
*/

//TEST #6 (input file not found)
/*
Input file not found. Exiting the program.
Press any key to continue . . .
*/