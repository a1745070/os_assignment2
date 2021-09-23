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
#include <climits>
using namespace std;

const int PRINT_LOG = 0; // print detailed execution trace
int count_customers = 0;

class Customer
{
public:
    string name;
    int customer_id;
    int priority;
    int arrival_time;
    int burst_time; // slots requested
    int wait_time;
    float ratio;

    Customer(string par_name, int par_customer_id, int par_priority, int par_arrival_time, int par_slots_requested)
    {
        name = par_name;
        customer_id = par_customer_id;
        priority = par_priority;
        arrival_time = par_arrival_time;
        burst_time = par_slots_requested;
        wait_time = -1;
        ratio = -1;
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
    deque<Customer> &customers)
{
    string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        Customer customer_from_file(name, customer_id, priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);

        // new customer arrival event
        Event arrival_event(arrival_time, customer_id);
        arrival_events.push_back(arrival_event);

        customer_id++;
        count_customers++;
    }
}

void print_state(
    ofstream &out_file,
    int current_time,
    int current_id,
    const deque<Customer> &customers,
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
    for (int i = 0; i < customers.size(); i++)
    {
        cout << "\t" << customers[i].priority << ", " << customers[i].customer_id << ", ";
    }
    cout << '\n';
    for (int i = 0; i < customer_queue.size(); i++)
    {
        cout << "\t" << customer_queue[i] << ", ";
    }
    cout << '\n';
}

/*
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
*/

void hrrn(deque<Customer> &customers, int current)
{
    // loop through customer queue and find the highest response ratio
    float maxi = -1;
    for(int i=0; i<customers.size(); i++)
    {
        // calculate current response ratio
        customers[i].wait_time = current - customers[i].arrival_time;
        customers[i].ratio = (customers[i].wait_time + customers[i].burst_time) / customers[i].burst_time;

        // keep track of max value
        if(customers[i].ratio > maxi)
        {
            maxi = customers[i].ratio;
        }
    }

    Customer cmp = NULL;
    int index = -1;
    for(int i=0; i<customers.size(); i++)
    {
        // find the customer(s) with the highest response ratio 
        if(customers[i].ratio == maxi)
        {
            if(cmp == NULL)
            {
                cmp = customers[i];
                index = i;
            }
            // calculate shortest job if more than one customers with same ratio
            else if (customers[i].burst_time < cmp.burst_time)
            {
                cmp = customers[i];
                index = i;
            }
            
        }
    }   

    swap(customers[0], customers[index]);


}




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
    deque<Customer> customers; // information about each customer

    // read information from file, initialize customers queue
    initialize_system(in_file, arrival_events, customers);

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

    // sjf_scheduling(high_priority_customers);

    cout<<"Priority 0 after sjf:"<<endl;
    for(int j=0; j<high_priority_customers.size(); j++)
    {
        cout<<high_priority_customers[j].customer_id<<endl;
    }

    // sjf_scheduling(low_priority_customers);

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
    bool all_done = false;
    for(int time=0; !all_done; time++)
    {
        if(!customers.empty())
        {
            queue.push_back(customers[0].customer_id);
            customers.pop_front();
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
        print_state(out_file, time, current_id, customers, queue);
        all_done = (customers.empty() && queue.empty() && current_id == -1);
    }*/

    return 0;
}