#include <iostream>
#include <set>
#include <map>

using namespace std;

int main(){
    int no_of_transactions, friends;
    cout << "Please input no_of_transactions & friends:\n";
    cout << "Ex:3 3 means there are three transactions and three person\n";
    cin >> no_of_transactions >> friends;

    string x,y;
    int amount;

    map<string, int>net; //key:person value:credit

    int onot = no_of_transactions;
    int counter = 1;
    cout << "Please input "<< no_of_transactions <<" transactions.\n";
    cout << "Ex: Tim Danny 100 (Tim will pay $100 to Danny)\n";
    while (no_of_transactions--){
        cout << "No." << counter++ << " transactions:";
        cin >> x >> y >> amount;

        if (net.count(x) == 0){
            net[x] = 0;
        }

        if (net.count(y) == 0){
            net[y] = 0;
        }

        net[x] -= amount;
        net[y] += amount;
    }

    multiset< pair<int, string> > m; // m = {(amount 1,person 1), (amount 2,person 2), ...}
    for (auto p: net){
        string person = p.first;
        int amount = p.second;

        if (net[person] != 0){
            m.insert( make_pair(amount, person) );
        }
    }

    int count = 0;
    while(!m.empty()){
        auto low = m.begin();
        auto high = prev( m.end() );

        int debit = low->first;
        string debit_person = low->second;

        int credit = high->first;
        string credit_person = high->second;

        m.erase(low);
        m.erase(high);

        int settlement_amount = min(-debit, credit);
        debit += settlement_amount;
        credit -= settlement_amount;

        cout << debit_person << " will pay " << settlement_amount << " to " << credit_person << endl;

        if (debit != 0){
            m.insert( make_pair(debit, debit_person) );
        }

        if (credit != 0){
            m.insert( make_pair(credit, credit_person) );
        }

        count += 1;
    }
    cout << endl;
    cout << "Without Algorithm it took: " << onot << " transacions" << endl;
    cout << "With Algorithm it took: " << count << " transacions";
    cout << endl;

}
