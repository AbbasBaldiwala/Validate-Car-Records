//#include <iostream>
//#include <sstream>
//#include <iomanip>
//#include "CarRecord.h" 
//
//const int SETW_ID = 15, SETW_MODEL = 9, SETW_QUANTITY = 12, SETW_PRICE = 14;
//
//using namespace std;
//Record::Record() {
//	SetRecord("", "", 0, 0);
//}
//
//void Record::SetRecord(string ID, string modelName, int quantityOnHand, double cost) {
//	carID = ID;
//	model = modelName;
//	quantity = quantityOnHand;
//	price = cost;
//}
//
//string Record::toString() const{
//	stringstream ss;
//	ss << setw(SETW_ID) << left << carID << setw(SETW_MODEL) << model << setw(SETW_QUANTITY) << right << quantity << setw(SETW_PRICE) << fixed << setprecision(2) << price << endl;
//	return ss.str();
//}
