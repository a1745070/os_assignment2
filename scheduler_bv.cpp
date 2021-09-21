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
    int slots_requested; // how many time slots are needed
    int playing_since;
    int finish_time;

    Customer(string par_name, int par_customer_id, int par_priority, int par_arrival_time, int par_slots_requested)
    {
        name = par_name;
        customer_id = par_customer_id;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_requested = par_slots_requested;
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

bool comparison(Customer a, Customer b)
{
    return (a.priority > b.priority);
}

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


void sjf_scheduling(deque<Customer> &customers)
{
    int total_num_of_customers = count_customers - 1;
    
    //Find the index of first high priority customer
    int flag;
    for(int i=0; i<customers.size(); i++)
    {   
        if(customers[i].priority == 0)
        {
            flag = i;
            cout<<"flag: "<<flag<<endl;
            break;
        }
    }

    int done;
    int current_time;
    int index;
    int min;

    index = -1;
    done = flag;
    current_time = 0;
    min = INT_MAX;

    //SJF For High Priority Customers
    while(done < total_num_of_customers)
    {
        for(int i=flag; i<customers.size(); i++)
        {
            if(customers[i].arrival_time <= current_time)
            {
                if(customers[i].slots_requested < min && customers[i].finish_time == 0)
                {
                    index = i;
                    cout<<"index: "<<index<<" ";
                    min = customers[i].slots_requested;
                    cout<<"current min: "<<min<<endl;
                }
            }
        }

        if(index >= 0)
        {
            done++;
            min = INT_MAX;
            current_time = current_time + customers[index].slots_requested;
            customers[index].finish_time = current_time;
            index = -1;
        }
        else
        {
            current_time++;
        }
    }
    

    index = -1;
    done = 0;
    current_time = 0;
    min = INT_MAX;

    //SJF for Regular Priority Customers
    while(done < flag)
    {
        for(int i=0; i<flag; i++)
        {
            if(customers[i].arrival_time <= current_time)
            {
                if(customers[i].slots_requested < min && customers[i].finish_time == 0)
                {
                    index = i;
                    min = customers[i].slots_requested;
                }
            }
        }

        if(index >= 0)
        {
            done++;
            min = INT_MAX;
            current_time = current_time + customers[index].slots_requested;
            customers[index].finish_time = current_time;
            index = -1;
        }
        else
        {
            current_time++;
        }
    }
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
    sort(customers.begin(), customers.end(), comparison);
    sjf_scheduling(customers);


/************************************************************************/
/*                   Copy results to queue for printing                 */
/************************************************************************/ 

/*    int current_id = -1; // who is using the machine now, -1 means nobody
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
