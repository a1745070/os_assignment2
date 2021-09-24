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
using namespace std;

const int TIME_ALLOWANCE = 8;  // allow to use up to this number of time slots at once
const int PRINT_LOG = 0; // print detailed execution trace

class Customer
{
public:
    string name;
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int playing_since;
<<<<<<< HEAD
=======
    int finish_time;
    int turnaround_time;
    int wait_time;
>>>>>>> 3c089d4f86f90058398b71d05ca1bbad36cbf25f

    Customer(string par_name, int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        name = par_name;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        playing_since = -1;
    }
};

class Event
{
public:
    int event_time;
    int customer_id;  // each event involes exactly one customer

    Event(int par_event_time, int par_customer_id)
    {
        event_time = par_event_time;
        customer_id = par_customer_id;
    }
};

void initialize_system(
    ifstream &in_file,
    deque<Event> &arrival_events,
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

        // new customer arrival event
        Event arrival_event(arrival_time, customer_id);
        arrival_events.push_back(arrival_event);

        customer_id++;
    }
}

void print_state(
    ofstream &out_file,
    int current_time,
    int current_id,
    const deque<Event> &arrival_events,
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
    for (int i = 0; i < arrival_events.size(); i++)
    {
        cout << "\t" << arrival_events[i].event_time << ", " << arrival_events[i].customer_id << ", ";
    }
    cout << '\n';
    for (int i = 0; i < customer_queue.size(); i++)
    {
        cout << "\t" << customer_queue[i] << ", ";
    }
    cout << '\n';
}

<<<<<<< HEAD
=======
void sjf_scheduling(deque<Customer> &customers)
{
    for(int i=0; i<customers.size(); i++)
    {
        for(int j=0; j<customers.size()-i-1; j++)
        {
            if(customers[j].arrival_time > customers[j+1].arrival_time)
            {
                swap(customers[j], customers[j+1]);
            }
        }
    }
    
    customers[0].finish_time = customers[0].arrival_time + customers[0].slots_requested;
    customers[0].turnaround_time = customers[0].finish_time - customers[0].arrival_time;
    customers[0].wait_time = customers[0].turnaround_time - customers[0].slots_requested;
    
    int temp, val;

    for(int i=1; i<customers.size(); i++)
    {
        temp = customers[i-1].finish_time;
        int low = customers[i].slots_requested;
        for(int j=i; j<customers.size(); j++)
        {
            if(temp <= customers[j].arrival_time && low >= customers[j].slots_requested)
            {
                low = customers[j].slots_requested;
                val = j;
            }
        }
        customers[val].finish_time = temp + customers[val].slots_requested;
        customers[val].turnaround_time = customers[val].finish_time - customers[val].arrival_time;
        customers[val].wait_time = customers[val].turnaround_time - customers[val].slots_requested;
        swap(customers[val], customers[i]);
    }
}


>>>>>>> 3c089d4f86f90058398b71d05ca1bbad36cbf25f
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

    deque<Event> arrival_events; // new customer arrivals
    vector<Customer> customers; // information about each customer

    // read information from file, initialize events queue
    initialize_system(in_file, arrival_events, customers);

<<<<<<< HEAD
    int current_id = -1; // who is using the machine now, -1 means nobody
    int time_out = -1; // time when current customer will be preempted
    deque<int> queue; // waiting queue

    // step by step simulation of each time slot
=======
    //Seperates Customers into two queues based on their priority
    deque<Customer> high_priority_customers;
    deque<Customer> low_priority_customers;
    
    for(int i=0; i<customers.size(); i++)
    {
        if(customers[i].priority == 0)
        {
            high_priority_customers.push_back(customers[i]);
        }
        else
        {
            low_priority_customers.push_back(customers[i]);
        }
    }


    cout<<"Priority 0 before sjf:"<<endl;
    for(int j=0; j<high_priority_customers.size(); j++)
    {
        cout<<high_priority_customers[j].customer_id<<endl;
    }

    cout<<"Priority 1:"<<endl;
    for(int j=0; j<low_priority_customers.size(); j++)
    {
        cout<<low_priority_customers[j].customer_id<<endl;
    }

    sjf_scheduling(high_priority_customers);

    cout<<"Priority 0 after sjf:"<<endl;
    for(int j=0; j<high_priority_customers.size(); j++)
    {
        cout<<high_priority_customers[j].customer_id<<endl;
    }

    sjf_scheduling(low_priority_customers);

    cout<<"Priority 1 after sjf:"<<endl;
    for(int j=0; j<low_priority_customers.size(); j++)
    {
        cout<<low_priority_customers[j].customer_id<<endl;
    }


/***********************************************************************
/*                   Copy results to queue for printing                 */
/************************************************************************/ 

   /* int current_id = -1; // who is using the machine now, -1 means nobody
    deque<int> queue; // waiting queue
>>>>>>> 3c089d4f86f90058398b71d05ca1bbad36cbf25f
    bool all_done = false;
    for (int current_time = 0; !all_done; current_time++)
    {
        // welcome newly arrived customers
        while (!arrival_events.empty() && (current_time == arrival_events[0].event_time))
        {
            queue.push_back(arrival_events[0].customer_id);
            arrival_events.pop_front();
        }
<<<<<<< HEAD

        // check if we need to take a customer off the machine
        if (current_id >= 0)
=======
        if(current_id >= 0)
>>>>>>> 3c089d4f86f90058398b71d05ca1bbad36cbf25f
        {
            if (current_time == time_out)
            {
                int last_run = current_time - customers[current_id].playing_since;
                customers[current_id].slots_remaining -= last_run;
                if (customers[current_id].slots_remaining > 0)
                {
                    // customer is not done yet, waiting for the next chance to play
                    queue.push_back(current_id);
                }
                current_id = -1; // the machine is free now
            }
        }
<<<<<<< HEAD

        // if machine is empty, schedule a new customer
        if (current_id == -1)
=======
        if(current_id == -1)
>>>>>>> 3c089d4f86f90058398b71d05ca1bbad36cbf25f
        {
            if (!queue.empty()) // is anyone waiting?
            {
                current_id = queue.front();
                queue.pop_front();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
        }
        print_state(out_file, current_time, current_id, arrival_events, queue);

        // exit loop when there are no new arrivals, no waiting and no playing customers
        all_done = (arrival_events.empty() && queue.empty() && (current_id == -1));
    }

    return 0;
}