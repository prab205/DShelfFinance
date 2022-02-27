#include <iostream>
#include <cstdlib>
using namespace std;

float creatorRPercentCut, authorPercentCut, creatorIPercentCut;
float authorProfit;
int noOfAuthor;
int count = 1, month = 0;
int initialPrice[3] = {700, 350, 200};

float gold[2] = {0, 0}, //initial 15
silver[2] = {0, 0}, //initial 20
bronze[2] = {0, 0}; //initial 70

float goldstat[2] = {0,0};
float silverstat[2] = {0,0};
float bronzestat[2] = {0,0};

double investmentCost = (1597.2 + 240) * 119.45;

float profitEarned = 0;


void increaseCostNoProfit (float * state, int x) {
    float rate = 1/(1-authorPercentCut-creatorRPercentCut);
    //state[1] = (state[1] * rate ) * (1 + x/10);

    gold[1] = gold[1] * rate * (1 + 2/10);
    silver[1] = silver[1] * rate * (1 + 1/10);
    bronze[1] = bronze[1] * rate;
    return;
}

void sell (float * status, float * statusStat, bool * statusSale, int rate) { // rate = 2,1,0
    if (status[0] > 0) {
        if (*statusSale) {
            profitEarned += status[0] * status[1] * creatorIPercentCut;
            authorProfit += status[1] * status[0] * (1 - creatorIPercentCut);
        }
        else {
            profitEarned += status[0] * status[1] * creatorRPercentCut;
            authorProfit += status[1] * status[0] * authorPercentCut;
        }
        statusStat[1] += status[0]; //
        if (statusStat[0] < statusStat[1]) {
            *statusSale = 0;
            statusStat[1] = status[0];
            increaseCostNoProfit(status, rate);
        }
    }
}

void monthend() {
    //cout << "\nMonth End\n";
    //cout << "\t" << profitEarned  << "\t" << authorProfit/noOfAuthor << "\t" << goldstat[1] << "\t" << silverstat[1] << "\t" << bronzestat[1] << "\t" << goldstat[1] + silverstat[1] + bronzestat[1] << "\n\n\n";
    bronze[0] += 2;
    silver[0] += 1;
    gold[0] += 1;
    goldstat[0] += 5;
    silverstat[0] += 15;
    bronzestat[0] += 50;
    month ++;
    profitEarned += 1370 + (500 * month); //some initial sale increasing every month (2,7,15)
    authorProfit += 4 * (1370 + (500 * month));
}

int main() {
    creatorRPercentCut = 0.02;
    authorPercentCut = 0.08;
    creatorIPercentCut = 0.20;

    bool bInitialSale = 1;
    bool sInitialSale = 1;
    bool gInitialSale = 1;

    noOfAuthor = 5;

    goldstat[0] = 5 * noOfAuthor; //initial 10
    silverstat[0] = 15 * noOfAuthor; // initial 20
    bronzestat[0] = 50 * noOfAuthor; // initial 70

    gold[0] = 5; gold[1] = 700;
    silver[0] = 12; silver[1] = 350;
    bronze[0] = 30; bronze[1] = 200;

    //initial sale
    profitEarned = (gold[1]*gold[0] + silver[1]*silver[0] + bronze[1]*bronze[0]) * creatorIPercentCut;
    authorProfit = (gold[1]*gold[0] + silver[1]*silver[0] + bronze[1]*bronze[0]) * (1 - creatorIPercentCut);

    //tracking total no of books sold
    goldstat[1] = gold[0];
    silverstat[1] = silver[0];
    bronzestat[1] = bronze[0];

    //for
    int j=2;

    gold[0] = 0;
    silver[0] = 0;
    bronze[0] = 2;


    /****** Selling of books ******/
    while (investmentCost > profitEarned) {

        if (gold[0] > 0) { sell(gold, goldstat, &gInitialSale, 2); }

        if (silver[0] > 0) { sell(silver, silverstat, &sInitialSale, 1); }

        if (bronze[0] > 0) { sell(bronze, bronzestat, &bInitialSale, 0);}

        cout <<  "day" << count << "\t" << gold[0] << "\t" << silver[0] << "\t" << bronze[0] << "\n";
        cout <<  "day" << count << "\t" << gold[1] << "\t" << silver[1] << "\t" << bronze[1] << "\n";

        if (count % 30 == 0) {
            monthend();
        }

        //selling 1 gold every week
        if (count%7 == 0 ) { gold[0] = 1;}
        else { gold[0] = 0;}

        //selling 1 silver every 4 days
        if (count % 4 == 0) { silver[0] = 1;}
        else { silver[0] = 0;}

        count ++;

        /************* Limiting infinite price growth ***************/
        if (gold[1] > 10 * initialPrice[0] || silver[1]> 10 * initialPrice[1] || bronze[1] > 10 * initialPrice[2]) {
            gold[1] = initialPrice[0] * j;
            silver[1] = initialPrice[1] * j;
            bronze[1] = initialPrice[2] * j;
            j++;
        }

        //if price increases too much, goes to base value
        if (j > 5) {
            j= 1;
            gold[1] = initialPrice[0] * j;
            silver[1] = initialPrice[1] * j;
            bronze[1] = initialPrice[2] * j;
        }

        //price of lower status can't be greater that higher status
        if (bronze[1] > silver[1]) {
            silver[1] = 1.3 * bronze[1];
        }
        if (silver[1] > gold[1]) {
            gold[1] = 1.3 * silver[1];
        }

    }

    cout << "\n\nCreator Initial Percent Cut = " << creatorIPercentCut * 100 << "\n";
    cout << "Creator Resale Percent Cut = " << creatorRPercentCut * 100 << "\n";
    cout << "Author Percent Cut = " << authorPercentCut * 100<< "\n" ;
    cout << "Initial investment = " << investmentCost << "\n" << "Gross Profit = " << profitEarned << "\n";
    cout << "Royalty obtained by a single author" << authorProfit/noOfAuthor;

    return 0;
}

