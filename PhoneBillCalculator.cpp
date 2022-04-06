/*
AUTHOR: mitlex
TITLE: Phone Bill Calculator
DATE: 28/10/2021
VERSION: v1
INPUT: Number of minutes used
OUTPUT: Total billed amount in pounds sterling

Extra specification features:
    - display breakdown of rates and discounts to user
    - option to calculate landline or mobile bill

Discount brackets I have chosen for the phone company (For both mobile and landline):

Up to and including 100 mins (discount 0 bracket)   15p per min (mobile); 10p per min (landline)
101 <= minutes <= 200 (discount 1 bracket)          14.25p per min (mobile); 9.5p per min (landline)
201 <= minutes <= 400 (discount 2 bracket)          13.5p per min (mobile); 9p per min (landline)
401 <= minutes (discount 3 bracket)                 12.75p per min (mobile); 8.5p per min (landline)

The above rates and minute ranges can be changed by the company if they want via the variable declarations in main().
However, as stated in the policy below, the minute ranges will be equal for both landline and mobile.

Note: the company has a policy in place that states the range of minutes for discounts for landline and mobile are equal,
therefore I have not made seperate variables for the number of minutes per discount bracket for both landline and mobile.
You can still change the bracket range values but not for mobile and landline individually.

Description of key choices and assumptions in algorithm:

I set console precision to 2 decimal places to guarantee a £X.XX pounds.pence format output.
I use the ceil function to ensure that the 3rd decimal place rounds up
e.g. Input 150 minutes for mobile bill, output should be £22.125, rounding up to £22.13.
(assuming rate = 15p for first 100 mins, 14.25p for remaining 50 mins)
Without the ceil function, this rounds down to £22.12.

With regard to a user inputting something like 50.9 mins, I just allow it to round down to 50mins.

algorithm explanation:

1. Show the user the mobile and landline rates.
2. Get user input asking for type of bill they want to calculate and number of minutes they have used.
3. Calculate the bill.

Calculation explanation:

The number of minutes determines which discount "bracket" the minutes fall into.
The idea is to multiply the rate for each bracket by the number of minutes that fall into that bracket.
For example, if I used 250 minutes on mobile:

The first 100 minutes are charged at 15p per minute, the second 100 at 14.25p per minute, and the last 50 at 13.5p per minute.
Then we simply sum these values together, and convert the total pence to pounds to get the total bill and return this to the user.

If the number of minutes lies between 101 and 200, then we already know that the first 100 minutes are charged at 15p
The number of minutes remaining (between 101 and 200) is flexible however, and to deal with this, we subtract the upper limit of the
non-discounted minutes bracket (100) from the total number of minutes, leaving us with the number of minutes to be charged with a discount (14.25p)

Since I have made the brackets flexible, I need to deal with this in the calculation.
For example, if I changed the second bracket limit to 250 minutes:

Up to and including 100 mins, charge 15p (discount 0 bracket)
101 <= minutes <= 250, charge 14.25p (discount 1 bracket)
251 <= minutes <= 400, charge 13.5p (discount 2 bracket)
401 <= minutes, charge 12.75p (discount 3 bracket)

I can make the brackets flexible by simply subtracting the upper limits of consequent brackets from each other.
If I input 275 minutes with the above brackets, the minutes charged at 14.25p is the difference between the no discount bracket limit (100)
and the first discount bracket limit (250) = 150 minutes. Then, since 275 < second discount bracket limit, we can just subtract the total
minutes (275) from the first discount bracket limit (250) to get the remaining 25 minutes to be charged at 13.5p per minute.
*/

#include <iostream>
using namespace std;

//print mobile and landline rates
void print_rates(const double rate, const double d1, const double d2, const double d3, const int d0_lim, const int d1_lim, const int d2_lim)
{
    cout << "Minutes spent up to and including " << d0_lim << " minutes are charged at " << rate << "p per minute." << endl;
    cout << "Minutes spent from " << d0_lim + 1 << " to including " << d1_lim << " minutes are charged at " << d1 << "p per minute." << endl;
    cout << "Minutes spent from " << d1_lim + 1 << " to including " << d2_lim << " minutes are charged at " << d2 << "p per minute." << endl;
    cout << "Minutes spent from " << d2_lim + 1 << " minutes and beyond are charged at " << d3 << "p per minute." << endl;
}

//display error message
//clear terminal for input re-entry - general idea credit https://www.youtube.com/watch?v=m2P5A4nR51g
void error()
{
    cout << "Invalid input, please try again: " << endl;
    cin.clear();
    cin.ignore(256, '\n');
}

//ask user if mobile or landline bill
int get_bill_type()
{
    int bill_type;

    cout << endl << "Would you like to calculate your mobile or land line bill?" << endl;
    cout << endl << "Enter 1 for mobile bill or 0 for landline bill: ";
    while (!(cin >> bill_type) || (bill_type != 1) && (bill_type != 0)) //protect vs. non-number input or input not 1 or 0
    {
        error();
    }

    return bill_type;
}

//get minutes used from user
int get_minutes()
{
    int mins;

    cout << endl << "Please enter your total minutes used: " << endl;
    while (!(cin >> mins) || mins < 0) //protect vs. non-number or negative input
    {
        error();
    }

    return mins;
}

//takes total minutes, standard rate and discount rates
//calculates and returns total bill
//note I had to cast the variables to double inside the function to prevent "Arithmetic Overflow" green underline error
//you can see it if you remove the double cast from any of the variables
double calculate_bill(int mins, const double rate, const double d1, const double d2, const double d3, const int d0_lim, const int d1_lim, const int d2_lim)
{
    double total_bill;

    if (mins <= d0_lim) {
        total_bill = ceil(mins * rate);
    }
    else if (mins <= d1_lim) {
        total_bill = ceil((d0_lim * rate) + ((double(mins) - d0_lim) * d1));
    }
    else if (mins <= d2_lim) {
        total_bill = ceil((d0_lim * rate) + ((double(d1_lim) - d0_lim) * d1) + ((double(mins) - d1_lim) * d2));
    }
    else { //mins > d2_lim
        total_bill = ceil((d0_lim * rate) + ((double(d1_lim) - d0_lim) * d1) + ((double(d2_lim) - d1_lim) * d2) + ((double(mins) - d2_lim) * d3));
    }
    return total_bill / 100.0; //convert to pounds
}

int main()
{
    const double mob_rate = 15, mob_d1 = mob_rate * 0.95, mob_d2 = mob_rate * 0.90, mob_d3 = mob_rate * 0.85; //mobile rates/discounted rates
    const double lline_rate = 10, lline_d1 = lline_rate * 0.95, lline_d2 = lline_rate * 0.90, lline_d3 = lline_rate * 0.85; //landline rates/disc rates
    const int d0_limit = 100, d1_limit = 200, d2_limit = 400; //discount bracket limits (up to 100 mins no discount etc.). Changeable.
    int minutes;
    int bill_type; //mobile or landline

    //welcome message, print rates
    cout << "Welcome to the Phone Bill calculator!" << endl << endl;
    cout << "Mobile rates: " << endl;
    print_rates(mob_rate, mob_d1, mob_d2, mob_d3, d0_limit, d1_limit, d2_limit);
    cout << endl;
    cout << "Landline rates: " << endl;
    print_rates(lline_rate, lline_d1, lline_d2, lline_d3, d0_limit, d1_limit, d2_limit);

    //ask mobile or landline, and number of minutes
    bill_type = get_bill_type();
    minutes = get_minutes();

    //set output precision to 2dp
    cout.setf(ios::fixed);
    cout.precision(2);

    //Calc + output total bill. £ (\x9C) output credit https://stackoverflow.com/questions/3831289/inserting-a-%C2%A3-sign-in-an-output-string-in-c

    if (bill_type == 1) //mobile
    {
        cout << endl << "The total bill for your mobile is: \x9C";
        cout << calculate_bill(minutes, mob_rate, mob_d1, mob_d2, mob_d3, d0_limit, d1_limit, d2_limit) << endl;
    }
    else //landline
    {
        cout << endl << "The total bill for your landline is: \x9C";
        cout << calculate_bill(minutes, lline_rate, lline_d1, lline_d2, lline_d3, d0_limit, d1_limit, d2_limit) << endl;
    }

    return 0;
}



