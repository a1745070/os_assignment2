// a1768127, Shue Hong, Lee
// a1761407, Kwan Han, Li
// a1745070, Archie, Verma
// OCTOPUS
/*
created by Andrey Kan
andrey.kan@adelaide.edu.au
2021
*/

#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

const int TIME_ALLOWANCE = 8;  // allow to use up to this number of time slots at once
const int PRINT_LOG = 0; // print detailed execution trace
const int run = 32;

class Customer
{
public:
    string name;
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int playing_since;

    Customer(string par_name, int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        name = par_name;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        playing_since = -1;
    }
};

class CID_and_CPriority
{
public:
    int priorities;
    int customer_id;  // each CID_and_CPriority involes exactly one customer

    CID_and_CPriority(int par_priorities, int par_customer_id)
    {
        priorities = par_priorities;
        customer_id = par_customer_id;
    }
};

bool comparison(CID_and_CPriority a, CID_and_CPriority b)
{
    return (a.priorities > b.priorities);
}

void initialize_system(
    ifstream &in_file,
    deque<CID_and_CPriority> &CIDs_and_CPriorities,
    vector<Customer> &customers)
{
    string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        Customer customer_from_file(name, priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);

        // new customer CID_and_CPriority
        CID_and_CPriority new_CID_and_CPriority(priority, customer_id);
        CIDs_and_CPriorities.push_back(new_CID_and_CPriority);

        customer_id++;
    }
}

void print_state(
    ofstream &out_file,
    int current_time,
    int current_id,
    const deque<CID_and_CPriority> &CIDs_and_CPriorities,
    const deque<int> &customer_queue)
{
    /************If (PRINT_LOG != 0), the program will not output .txt file*************/
    
    out_file << current_time << " " << current_id << '\n';
    if (PRINT_LOG == 0)
    {
        return;
    }
    
    /***********************************************************************************/
    
    cout << current_time << ", " << current_id << '\n';
    for (int i = 0; i < CIDs_and_CPriorities.size(); i++)
    {
        cout << "\t" << CIDs_and_CPriorities[i].priorities << ", " << CIDs_and_CPriorities[i].customer_id << ", ";
    }
    cout << '\n';
    for (int i = 0; i < customer_queue.size(); i++)
    {
        cout << "\t" << customer_queue[i] << ", ";
    }
    cout << '\n';
}


//void sjf_scheduling(){}


// process command line arguments
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Provide input and output file names." << endl;
        return -1;
    }
    ifstream in_file(argv[1]);
    ofstream out_file(argv[2]);
    if ((!in_file) || (!out_file))
    {
        cerr << "Cannot open one of the files." << endl;
        return -1;
    }

    deque<CID_and_CPriority> CIDs_and_CPriorities; // new customer arrivals
    vector<Customer> customers; // information about each customer

    // read information from file, initialize CID_and_CPrioritys queue
    initialize_system(in_file, CIDs_and_CPriorities, customers);
    sort(CIDs_and_CPriorities.begin(), CIDs_and_CPriorities.end(), comparison);
    
    int current_id = -1; // who is using the machine now, -1 means nobody
    deque<int> queue; // waiting queue

    bool all_done = false;
    for(int time=0; !all_done; time++)
    {
        if(!CIDs_and_CPriorities.empty())//&& (time == CIDs_and_CPriorities[0].CID_and_CPriority_time))
        {
            queue.push_back(CIDs_and_CPriorities[0].customer_id);
            CIDs_and_CPriorities.pop_front();
        }

        if(current_id >= 0)
        {
            current_id = -1;
        }

        if(current_id == -1)
        {
            if(!queue.empty())
            {
                current_id = queue.front();
                queue.pop_front();
            }
        }
        all_done = (CIDs_and_CPriorities.empty() && queue.empty() && current_id == -1);
        print_state(out_file, time, current_id, CIDs_and_CPriorities, queue);
    }
    return 0;
}
