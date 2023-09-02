#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;
const int MAX_NUM_RECORDS = 8, 
	SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14, SETPRECISION = 2,
	TABLE_LENGTH = SETW_ID + SETW_MODEL + SETW_PRICE + SETW_QUANTITY,
	MIN_PRICE = 5000, MIN_QUANTITY = 0, MIN_MODEL_LEN = 3,
	ID_CRIT_1_NUM_LETTERS = 2, ID_CRIT_2_NUM_ALPHANUMERIC = 4, ID_CRIT_3_NUM_DIGITS = 3,
	CORRECT_ID_LEN = ID_CRIT_1_NUM_LETTERS + ID_CRIT_2_NUM_ALPHANUMERIC + ID_CRIT_3_NUM_DIGITS;

class Record {
public:
	Record() {
		SetRecord("", "", 0, 0);
	}

	void SetRecord(string ID, string modelName, int quantityOnHand, double cost) {
		carID = ID;
		model = modelName;
		quantity = quantityOnHand;
		price = cost;
	}

	string toString() const {
		stringstream ss;
		ss << setw(SETW_ID) << left << carID 
			<< setw(SETW_MODEL) << model 
			<< setw(SETW_QUANTITY) << right << quantity 
			<< setw(SETW_PRICE) << fixed << setprecision(SETPRECISION) << price;
		return ss.str();
	}

private:
	string carID, model;
	int quantity;
	double price;
};

enum Menu {
	PRINT_VALID = 1, PRINT_INVALID = 2, QUIT = 3
};

//Gets the data from the input file and writes all invalid data to error file
void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec, int& numInvalidRec, string errorFileName);

//prints valid records to screen in table format
void PrintValidRecords(const Record validRec[MAX_NUM_RECORDS], int numValidRec, string border, string header); 

//prints invalid records to screen in table format
void PrintInvalidRecords(string border, string header, string errorFileName, int numInvalid);

bool IsValidID(string carID, string& errorMessage); //checks if the Car ID is valid

bool IsValidModel(string model, string& errorMessage); //checks if the Model Name is valid

bool IsValidQuantity(int quantity, string& errorMessage); //checks if the quantity is valid

bool IsValidPrice(double price, string& errorMessage); //checks if the price is valid

string ToUpper(string str); //returns string converted to upper case

void ClearInvalidInput(string errMsg); //clears cin, clears the keyboard buffer, prints an error message

int main() {
	int userChoice, numValidRec = 0, numInvalidRec = 0;
	stringstream border, header;
	Record validRecords[MAX_NUM_RECORDS];
	string errorFileName = "InvalidRecords.txt";
	GetData(validRecords, numValidRec, numInvalidRec, errorFileName);
	
	border << setfill('-') << setw(TABLE_LENGTH) << "";
	header << setw(SETW_ID) << left << "Car ID" << setw(SETW_MODEL) << "Model" << setw(SETW_QUANTITY) << right << "Quantity" << setw(SETW_PRICE+1) << "Price\n" << endl;
	
	do {
		cout << "\n\nMENU: \n"
			"1. PRINT VALID RECORDS\n"
			"2. PRINT INVALID RECORDS\n"
			"3. QUIT\n" << endl;
		cin >> userChoice;
		switch (userChoice) {
		case PRINT_VALID:
			PrintValidRecords(validRecords, numValidRec, border.str(), header.str());
			break;
		case PRINT_INVALID:
			PrintInvalidRecords(border.str(), header.str(), errorFileName, numInvalidRec);
			break;
		case QUIT:
			cout << "\nQUITTING..." << endl;
			break;
		default:
			ClearInvalidInput("INVALID MENU SELECTION");
		}
	} while (userChoice != QUIT);
	return 0;
}

void GetData(Record validRec[MAX_NUM_RECORDS], int& numValidRec, int& numInvalidRec, string errorFileName) {
	string tempCarID, tempModel, errMsg = "";
	int tempQuantityOnHand;
	double tempPrice;
	bool isValidID, isValidModel, isValidQuantity, isValidPrice;
	ifstream inFile("test.txt");
	ofstream errorFile(errorFileName);
	Record temp;
	if (!inFile) {
		cout << "Input file not found. Exiting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}	
	if (!errorFile) {
		cout << "Error file not found. Exiting the program." << endl;
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

		tempCarID = ToUpper(tempCarID);
		tempModel = ToUpper(tempModel);

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
			/*errorFile << setw(SETW_ID) << left << tempCarID  << setw(SETW_MODEL) << tempModel << setw(SETW_QUANTITY) << right 
				<< tempQuantityOnHand  << setw(SETW_PRICE) << fixed << setprecision(SETPRECISION) << tempPrice << " " << errMsg << endl;*/
			temp.SetRecord(tempCarID, tempModel, tempQuantityOnHand, tempPrice);
			errorFile << temp.toString() << " " << errMsg << endl;
			numInvalidRec++;
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
		cout << "\nNO VALID RECORDS FOUND.\n";
	}
	else {
		cout << "\nDISPLAYING " << numValidRec << " VALID RECORDS(UNSORTED)...\n";
		cout << border << "\n" << header;
		for (int i = 0; i < numValidRec; i++) {
			cout << validRec[i].toString() << endl;
		}
		cout << border << "\n";
	}
}

void PrintInvalidRecords(string border, string header, string errorFileName, int numInvalidRec) {
	ifstream invalidRecFile(errorFileName);
	if (!invalidRecFile) {
		cout << "Invalid Records file not found. Exiting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (invalidRecFile.peek() == EOF) { 
		cout << "\nNO INVALID RECORDS FOUND\n";
	}
	else {
		cout << "\nDISPLAYING " << numInvalidRec << " INVALID RECORDS...\n" 
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
		errorMessage += "INVALID QUANTITY [Can't be less than " + to_string(MIN_QUANTITY) + ".] ";
	}
	return quantity >= MIN_QUANTITY;
}

bool IsValidPrice(double price, string& errorMessage) {
	if (price <= MIN_PRICE) {
		errorMessage += "INVALID PRICE [Must be greater than $" + to_string(MIN_PRICE) + ".] ";
	}
	return price > MIN_PRICE;
}

string ToUpper(string str) {
	string upperCaseStr = "";
	for (int i = 0; i < str.length(); i++) {
		char ch = str[i];
		if (((ch >= 'a') && (ch <= 'z'))) {
			ch -= ('a' - 'A'); //converts char to upper case
		}
		upperCaseStr += ch;
	}
	str = upperCaseStr;
	return upperCaseStr;
}

void ClearInvalidInput(string errMsg) {
	cout << "\n" << errMsg << endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//TEST RUNS

// TEST #1 (Some Valid and Invalid Data)
/*
MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

sdfdsa

INVALID MENU SELECTION


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

1

DISPLAYING 10 VALID RECORDS(UNSORTED)...
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


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

2

DISPLAYING 15 INVALID RECORDS...
--------------------------------------------------
Car ID         Model        Quantity         Price

PSUSVF195      WRANGLER            7       1000.00 INVALID PRICE [Must be greater than $5000.]
CB6P96365      ESCALADE           -1      99999.99 INVALID QUANTITY [Can't be less than 0.]
ML8W6S179      JETTA              -4        234.00 INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
KHKQ95204      911                78      23145.00 INVALID MODEL [ Must start with a letter.]
DW2NRB367      R:X                67          0.00 INVALID MODEL [ Must consist of only alphanumeric characters.] INVALID PRICE [Must be greater than $5000.]
FU0R94012      2018CAMRY          -2       9999.00 INVALID MODEL [ Must start with a letter.] INVALID QUANTITY [Can't be less than 0.]
XS4R45397      A4                 -4      -2342.00 INVALID MODEL [Must be at least 3 characters long.] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
ALFOS5213      ROADSTER            2      12435.00 INVALID ID [ Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed).]
1B1293675      FOCUS             300       -324.00 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).] INVALID PRICE [Must be greater than $5000.]
123456789      CT250              -5      74829.00 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).] INVALID QUANTITY [Can't be less than 0.]
ABCDEFGHI      RAV4              -35         22.00 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
ALS;DA167      4RUBICON           92      30000.00 INVALID ID [ Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed).] INVALID MODEL [ Must start with a letter.]
GH32HS1232     A2                  6     -24000.00 INVALID ID [ Must be 9 characters long.] INVALID MODEL [Must be at least 3 characters long.] INVALID PRICE [Must be greater than $5000.]
VD234019P      M4                 -3      60000.00 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID MODEL [Must be at least 3 characters long.] INVALID QUANTITY [Can't be less than 0.]
ABCDED7121     2FX               -96         -5.00 INVALID ID [ Must be 9 characters long.] INVALID MODEL [ Must start with a letter.] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
--------------------------------------------------


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

3

QUITTING...

C:\Users\Abbas\source\repos\Lab1 Refresher\x64\Debug\Lab1 Refresher.exe (process 10644) exited with code 0.
Press any key to close this window . . .
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


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

1

DISPLAYING 8 VALID RECORDS(UNSORTED)...
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


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

2

DISPLAYING 15 INVALID RECORDS...
--------------------------------------------------
Car ID         Model        Quantity         Price

PSUSVF195      WRANGLER            7       1000.00 INVALID PRICE [Must be greater than $5000.]
CB6P96365      ESCALADE           -1      99999.99 INVALID QUANTITY [Can't be less than 0.]
ML8W6S179      JETTA              -4        234.00 INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
KHKQ95204      911                78      23145.00 INVALID MODEL [ Must start with a letter.]
DW2NRB367      R:X                67          0.00 INVALID MODEL [ Must consist of only alphanumeric characters.] INVALID PRICE [Must be greater than $5000.]
FU0R94012      2018CAMRY          -2       9999.00 INVALID MODEL [ Must start with a letter.] INVALID QUANTITY [Can't be less than 0.]
XS4R45397      A4                 -4      -2342.00 INVALID MODEL [Must be at least 3 characters long.] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
ALFOS5213      ROADSTER            2      12435.00 INVALID ID [ Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed).]
1B1293675      FOCUS             300       -324.00 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).] INVALID PRICE [Must be greater than $5000.]
123456789      CT250              -5      74829.00 INVALID ID [ First 2 characters must be Alpha(A - Z, letter O is not allowed).] INVALID QUANTITY [Can't be less than 0.]
ABCDEFGHI      RAV4              -35         22.00 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
ALS;DA167      4RUBICON           92      30000.00 INVALID ID [ Characters 3-6 must be alphanumeric(A - Z, letter O is not allowed).] INVALID MODEL [ Must start with a letter.]
GH32HS1232     A2                  6     -24000.00 INVALID ID [ Must be 9 characters long.] INVALID MODEL [Must be at least 3 characters long.] INVALID PRICE [Must be greater than $5000.]
VD234019P      M4                 -3      60000.00 INVALID ID [ Characters 7-9 must be numeric (0-9).] INVALID MODEL [Must be at least 3 characters long.] INVALID QUANTITY [Can't be less than 0.]
ABCDED7121     2FX               -96         -5.00 INVALID ID [ Must be 9 characters long.] INVALID MODEL [ Must start with a letter.] INVALID QUANTITY [Can't be less than 0.] INVALID PRICE [Must be greater than $5000.]
--------------------------------------------------


MENU:
1. PRINT VALID RECORDS
2. PRINT INVALID RECORDS
3. QUIT

3

QUITTING...

C:\Users\Abbas\source\repos\Lab1 Refresher\x64\Debug\Lab1 Refresher.exe (process 15928) exited with code 0.
Press any key to close this window . . .
*/

//TEST #6 (input file not found)
/*
Input file not found. Exiting the program.
Press any key to continue . . .
*/