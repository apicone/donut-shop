/*
Author: Angelina Picone
Date: 09/28/2023
Purpose: work as a point of sales for a donut shop via the terminal
Possible Future: create a user interface and work with that instead of the terminal
*/


using namespace std;
// LIBRARIES
#include <iostream>
#include <iomanip> 

// FUNCTION HEADERS

double getOrder(int* totalDon);
double getOne(double indiv, double doz, int index, string name, int* donArr);


/*
handles processing of payments
*/
class PaymentTransaction
{
private:
	double customer_owes = 0;
	void getCard();
	void getChange();
	int calcCoins(int coinsLeft, int val, string name);
public:
	PaymentTransaction(double owes)
	{
		customer_owes = owes;
	}
	void makePayment();
};

/*
MAIN
*/
int main() 
{
	const int DONUT_TYPES = 3;
	int totalDonuts[DONUT_TYPES];
	for(int i = 0; i < DONUT_TYPES; i++)
	{
		totalDonuts[i] = 0;
	}

	double totalRevenue = 0;
	do
	{
		double orderCost = getOrder(totalDonuts);
		totalRevenue += orderCost;
		cout << "Customer owes $" << fixed << setprecision(2) << orderCost << endl;
		PaymentTransaction pay = PaymentTransaction(orderCost);
		pay.makePayment();

		int t;
		cout << endl << endl <<"Enter a number to calculate another order: ";
		cin >> t;
	} while (!cin.fail());

	cout << endl << "--------------------" << endl << endl;;
	cout << "Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
	cout << "*******" << endl;
	cout << "ITEMS SOLD" << endl;
	cout << "Regular donuts - " << totalDonuts[0] << endl;
	cout << "Fancy donuts - " << totalDonuts[1] << endl;
	cout << "Apple fritters - " << totalDonuts[2] << endl;

}

/*
gets the order from the user, adds new order to total counts, returns cost for this order
@param integer arrary (accessed via pointer) of the total donuts for the time period
@return double (cost of donut purchase)
*/
double getOrder(int* totalDon)
{
	int indexCount = 0;
	double preTaxCost = 0;
	// regular donuts
	double regIndiv = .75;
	double regDoz = 7.99;
	preTaxCost += getOne(regIndiv, regDoz, indexCount, "Regular donuts", totalDon);
	indexCount++;

	// fancy donuts
	double fanIndiv = .85;
	double fanDoz = 8.49;
	preTaxCost += getOne(fanIndiv, fanDoz, indexCount, "Fancy donuts", totalDon);
	indexCount++;

	// apple fritters
	double appleIndiv = 1.50;
	int appleDoz = 12 * appleIndiv;
	preTaxCost += getOne(appleIndiv, appleDoz, indexCount, "Apple fritters", totalDon);
	indexCount++;

	// all
	double TAX = .075;
	return (preTaxCost * (1 + TAX));

}

/*
get user input for the number of donuts of a type ordered, returns the cost of that set of donuts
@param	double - cost of individual "donut"
@param	double - cost of 12 (1 dozen) "donuts"
@param	integer - index of "donut" type in the total array
@param	string - name of type of "donut"
@param	integer pointer - array of the amount ordered of each donut type
@return cost of donut type
*/
double getOne(double indiv, double doz, int index, string name, int* donArr)
{
	int count = 0;
	cout << name << " ordered: ";
	cin >> count;
	donArr[index] += count;
	return (count / 12) * doz + (count % 12) * indiv;
}

/*
get user input for type of payment, call upon other functions to handle processing of payment
@param	double - what the customer owes for their order
*/
void PaymentTransaction::makePayment()
{
	int type;
	do 
	{
		cout << "1) credit card" << endl << "2) cash" << endl;
		cout << "Type of payment: ";
		cin >> type;
	}
	while (type != 1 && type != 2);
	
	switch(type) 
	{
	case 1:
		// credit card
		getCard();
		break;
	case 2:
		// cash
		getChange();
		break;
	}
}

/*
processing of a card payment: 
	- get user input
	- verify card number
	- keep asking for card input until valid number
*/
void PaymentTransaction::getCard()
{
	bool valid;
	do {
		int card;
		cout << "Enter card number: ";
		cin >> card;
		// verify card
		int even = 0;
		int odd = 0;
		int counter = 0;
		while (card > 0)
		{
			int digit = card % 10;
			card /= 10;
			if (counter % 2 == 0)
			{
				even += digit;
			}
			else
			{
				int t = digit * 2;
				odd += t % 10 + t / 10;
			}
		}
		int sum = even + odd;
		valid = (sum % 10 == 0);
		if (valid)
		{		
			cout << "Card is valid." << endl;
		}
		else
		{
			cout << "Card is invalid. Try again." << endl;
		}
	} while (!valid);
}

/*
processing of cash payment:
	- get user input
	- compute what change is owed
	- compute & print what the appropriate amount of cash and each amount of coins is
@param double - the amount of money the customer owes
*/
void PaymentTransaction::getChange()
{
	// get user input
	cout << "Customer pays $";
	double payment;
	cin >> payment;
	double isOwed = payment - customer_owes;
	
	// print how much is owed
	if ((int)(isOwed * 100) == 0) { cout << "No change due - exact payment received"; }
	else 
	{
		cout << "Changed owed is $" << fixed << setprecision(2) << isOwed << " - ";

		// dollars
		int dollars = isOwed;
		int coins = (isOwed - dollars) * 100 + .5;
		if (dollars > 0) {
			cout << dollars;
			if (dollars == 1) { cout << " dollar"; }
			else { cout << " dollars"; }
			if (coins > 0) { cout << ", "; }
			else { cout << "."; }
		}

		//quarters
		coins = calcCoins(coins, 25, "quarter");

		// dimes
		coins = calcCoins(coins, 10, "dime");

		// nickels
		coins = calcCoins(coins, 5, "nickel");

		// pennies
		int pennies = coins;
		if (pennies == 1) { cout << pennies << " penny"; }
		else if (pennies > 1) { cout << pennies << " pennies"; }
	}
	cout << ".";
}

/*
@param	integer - amount of coins remaining
@param	integer - value of current coin
@param	string - name of current coin
@return	integer - amount of coins left
*/
int PaymentTransaction::calcCoins(int coinsLeft, int val, string name)
{
	// calculation
	int n = coinsLeft / val;
	coinsLeft = coinsLeft % val;
	if (n > 0) {
		// name portion
		cout << n << " " << name;
		if (n > 1) { cout << "s"; }
		// punctuation
		if (coinsLeft > 0) { cout << ", "; }
	}
	return coinsLeft;
}