#include <iostream>
#include <sstream>
#include <iomanip>
#include "Record.h"

int SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14;

using namespace std;
Record::Record() {
	SetRecord("", "", -1, -1);
}

void Record::SetRecord(string ID, string modelName, int quantityOnHand, double cost) {
	carID = ID;
	model = modelName;
	quantity = quantityOnHand;
	price = cost;
}

Record::~Record() {}

Record& Record::operator=(const Record& other) {
	if (this != &other) {
		carID = other.carID;
		model = other.model;
		quantity = other.quantity;
		price = other.price;
	}
	return *this;
}

string Record::toString() {
	stringstream ss;
	ss << setw(SETW_ID) << left << carID << setw(SETW_MODEL) << model << setw(SETW_QUANTITY) << right << quantity << setw(SETW_PRICE) << fixed << setprecision(2) << price << endl;
	return ss.str();
}

void Record::toUpper() {
	string uppercaseString = "";
	for (int i = 0; i < carID.length(); i++) {
		char s = carID[i];
		if (((s - 'a' >= 0) && (s - 'a' < 26))) {
			s -= 32;
		}
		uppercaseString += s;
	}
	carID = uppercaseString;
}