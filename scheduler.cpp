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

const int PRINT_LOG = 0; // print detailed execution trace
int count_customers = 0;
const int default_time_allowance = 4;
const int p0_promotion = 100;
const int p1_promotion = 400;   

class Customer
{
public:
    string name;
    int customer_id;
    int priority;
    int arrival_time;
    int slots_remaining; // slots requested
    int time_limit;
    int wait_time;
    int playing_since;
    float ratio;

    Customer(string par_name, int par_customer_id, int par_priority, int par_arrival_time, int par_slots_requested)
    {
        name = par_name;
        customer_id = par_customer_id;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_requested;
        time_limit = default_time_allowance;
        playing_since = -1;
        wait_time = -1;
        ratio = -1;
    }
};


void initialize_system(
    ifstream &in_file,
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

        customer_id++;
        count_customers++;
    }
}

void print_state(
    ofstream &out_file,
    int current_time,
    int current_id)
{
    out_file << current_time << " " << current_id << '\n';
    if (PRINT_LOG == 0)
    {
        return;
    }

    /*
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
    */
}


void hrrn(deque<Customer> &customers, int current)
{
    // loop through customer queue and find the highest response ratio
    float maxi = -1;
    for(int i=0; i<customers.size(); i++)
    {
        // calculate current response ratio
        customers[i].ratio = (customers[i].wait_time + customers[i].slots_remaining) / customers[i].slots_remaining;

        // keep track of max value
        if(customers[i].ratio > maxi)
        {
            maxi = customers[i].ratio;
        }
    }

    Customer *cmp = nullptr;
    int index = -1;
    for(int i=0; i<customers.size(); i++)
    {
        // find the customer(s) with the highest response ratio 
        if(customers[i].ratio == maxi)
        {
            if(cmp == nullptr)
            {
                cmp = &customers[i];
                index = i;
            }
            // calculate shortest job if more than one customers with same ratio
            else if (customers[i].slots_remaining < cmp->slots_remaining)
            {
                cmp = &customers[i];
                index = i;
            }  
        }
    }   

    swap(customers[0], customers[index]);
}

// set the running process time_out with current process
int addProcess(Customer* executing, int current_time)
{
    int time_out = 0;
    if (executing->time_limit > executing->slots_remaining)
    {
        // process can be finished within time limit
        time_out = current_time + executing->slots_remaining;
    }
    else
    {
        // set time out time
        time_out = current_time + executing->time_limit;
    }
    executing->playing_since = current_time;

    return time_out;
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

    deque<Customer> customers; // information about each customer

    // read information from file, initialize customers queue
    initialize_system(in_file, customers); 
    int current_id = -1;

    //Seperates Customers into two queues based on their priority
    std::deque<Customer> waiting_queue;
    std::deque<Customer> P0; // high priority
    std::deque<Customer> P1; // low priority


    // step by step simulation of each time slot
    bool all_done = false;
    int time_out = -1;
    Customer *executing = nullptr; // customer currently using the arcade
    for (int current_time = 0; !all_done; current_time++)
    {
        // welcome newly arrived customers
        while (!customers.empty() && (current_time == customers[0].arrival_time))
        {
            waiting_queue.push_back(customers[0]);
            customers.pop_front();

        }
        
        // check if we need to take a customer off the machine
        if (current_id >= 0)
        {
            if (current_time == time_out)
            {
                executing->slots_remaining -= (current_time - executing->playing_since);
                if (executing->slots_remaining > 0)
                {
                    // customer is not done yet, push to lower waiting queues according to priority
                    if(executing->priority == 0)
                    {
                        executing->time_limit += executing->time_limit;
                        P0.push_back(*executing);
                    }
                    else
                    {
                        executing->time_limit += executing->time_limit;
                        P1.push_back(*executing);
                    }   
                }
                // the machine is free now
                current_id = -1;
                executing = nullptr;
            }
        }

        // calculate waiting time
        if(!waiting_queue.empty())
        {
            for(int i=0; i<waiting_queue.size(); i++)
            {
                waiting_queue[i].wait_time ++;
            }

        }
        if(!P0.empty())
        {
            for(int i=0; i<P0.size(); i++)
            {
                P0[i].wait_time ++;
            }
        }
        if(!P1.empty())
        {
            for(int i=0; i<P1.size(); i++)
            {
                P1[i].wait_time ++;
            }
        }

        // if machine is empty, schedule a new customer
        if (current_id == -1)
        {
            // calculate the response ratio for P0 and P1
            if(!P0.empty())
            {
                hrrn(P0, current_time);
            }
            if(!P1.empty())
            {
                hrrn(P1, current_time);
            }

            // calculate main customer queue first
            if(!waiting_queue.empty())
            {
                current_id = waiting_queue.front().customer_id;
                executing = &waiting_queue.front();
                waiting_queue.pop_front();
                time_out = addProcess(executing, current_time);
            }
            else if(!P0.empty())
            {
                // push P0 to main queue
                current_id = P0.front().customer_id;
                executing = &P0.front();
                P0.pop_front();
                time_out = addProcess(executing, current_time);
            }
            else if(!P1.empty())
            {
                current_id = P1.front().customer_id;
                executing = &P1.front();
                P1.pop_front();
                time_out = addProcess(executing, current_time);
            }

          
            // check for P0 promotion
            if(!P0.empty() && P0.front().wait_time >= p0_promotion)
            {
                waiting_queue.push_back(P0.front());
                P0.pop_front();
            }
  
            // check for P1 promotion
            if(!P1.empty() && P1.front().wait_time >= p1_promotion)
            {
                P0.push_back(P1.front());
                P1.pop_front();
            }
        }

        print_state(out_file, current_time, current_id);

        // exit loop when there are no new arrivals, no waiting and no playing customers
        all_done = (customers.empty() && P0.empty() && P1.empty() && (current_id == -1));
    }

    return 0;
}