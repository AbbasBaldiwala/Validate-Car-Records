#ifndef RECORD_H
#define RECORD_H

using namespace std;

class Record {
public:

	Record();

	~Record();

	Record& operator=(const Record& other);

	void SetRecord(string, string, int, double);

	string toString();

	void toUpper();

private:
	string carID, model, errorMessage;
	int quantity;
	double price;
};

#endif
