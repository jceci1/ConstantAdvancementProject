#include <iostream>
#include <vector>
#include <set>
#include "counter.h"
#include "global_counter.h"
#include "thread.h"
#include "thread_scheduler.h"


/*Verify: Checks that all values 1-100 were processed exactly once.*/
void verify(const std::vector<counter*>& counters) {
    std::vector<int> all_values;
    
    //collect all processed values from all threads
    for(size_t i = 0; i < counters.size(); i++) {
        const std::vector<int>& values = counters[i]->getProcessedValues();
        for(size_t j = 0; j < values.size(); j++) {
            all_values.push_back(values[j]);
        }
    }

    //track which values we've seen
    std::set<int> seen;
    std::vector<int> duplicates;
    std::vector<int> oor_values;

    //check for duplicates and out-of-range values
    for(size_t i = 0; i < all_values.size(); i++) {
        int value = all_values[i];
        
        if(value < 1 || value > 100) {
            oor_values.push_back(value);
        }
        
        if(seen.count(value) > 0) {
            duplicates.push_back(value);
        }
        seen.insert(value);
    }

    //check formissing values
    std::vector<int> missing;
    for(int i = 1; i <= 100; i++) {
        if(seen.count(i) == 0) {
            missing.push_back(i);
        }
    }

    //print verification results
    std::cout << "\nVerification results:\n";
    
    if(seen.size() == 100 && duplicates.empty() && oor_values.empty()) {
        std::cout << "VERIFICATION PASSED\n";
        std::cout << "All values 1-100 present exactly once\n";
        std::cout << "Total count: " << all_values.size() << "\n";
    } else {
        std::cout << "VERIFICATION FAILED\n";
        
        //checking for duplicates
        if(duplicates.size() > 0) {
            std::cout << "Duplicated values: ";
            for(size_t i = 0; i < duplicates.size(); i++) {
                std::cout << duplicates[i] << " ";
            }
            std::cout << "\n";
        }
        
        //checking for out-of-range values
        if(oor_values.size() > 0) {
            std::cout << "Out-of-range values: ";
            for(size_t i = 0; i < oor_values.size(); i++) {
                std::cout << oor_values[i] << " ";
            }
            std::cout << "\n";
        }
        
        //checking for missing values
        if(missing.size() > 0) {
            std::cout << "Skipped values (" << missing.size() << "): ";
            for(size_t i = 0; i < missing.size(); i++) {
                std::cout << missing[i] << " ";
            }
            std::cout << "\n";
        }
        
        std::cout << "Unique count: " << seen.size() << "/100\n";
        std::cout << "Total processed: " << all_values.size() << "\n";
    }
}


int main() {
    int counter_type;
    unsigned int random_seed;

    std::cout << "Select counter type (1: Normal, 2: Deterministic): ";
    std::cin >> counter_type;
    std::cout << "Enter random seed (unsigned integer): ";
    std::cin >> random_seed;

//set up shared counter and scheduler
    global_counter shared_counter;
    thread_scheduler scheduler;
    bool use_delays = false;
    
    //generate the order that values will be assigned to threads
    std::vector<int> execution_order = thread_scheduler::generateOrder(random_seed);
    

    if(counter_type == 2) {
        scheduler.setEnabled(true);
        scheduler.setOrder(execution_order);
        std::cout << "\nDeterministic scheduling enabled with seed: " << random_seed << "\n\n";
    } else {
        use_delays = true;
        std::cout << "\nNormal scheduling with seed: " << random_seed << "\n\n";
    }


    std::vector<counter*> counters;
    std::vector<thread*> threads;

    for(int i = 0; i < 5; i++) {
        counter* task = new counter(&shared_counter, i, random_seed, use_delays, &scheduler);
        counters.push_back(task);

        thread* t = new thread(task);
        threads.push_back(t);
    }

    //assign values to threads based on execution order
    for(int i = 0; i < 100; i++) {
        int assigned_thread = execution_order[i];
        counters[assigned_thread]->assignValue(i + 1);
    }


    for(size_t i = 0; i < threads.size(); i++) {
        threads[i]->start();
    }

    for(size_t i = 0; i < threads.size(); i++) {
        threads[i]->join();
    }

    //print summary of what each thread did
    std::cout << "\nthread execution summary:\n";
    int total = 0;

    for(int i = 0; i < 5; i++) {
        const std::vector<int>& values = counters[i]->getProcessedValues();
        total += values.size();

        std::cout << "\nThread " << i << " (ID: " << threads[i]->getId() << "):\n";
        std::cout << "  Processed " << values.size() << " values\n";
        std::cout << "  Values: ";
        
        for(size_t j = 0; j < values.size(); j++) {
            std::cout << values[j];
            if(j < values.size() - 1) {
                std::cout << ", ";
            }
            
            //improves readability
            if((j + 1) % 20 == 0 && j < values.size() - 1) {
                std::cout << "\n          ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nTotal values processed across all threads: " << total << "\n";

   
    verify(counters);

    //cleanup
    for(int i = 0; i < 5; i++) {
        delete threads[i];
        delete counters[i];
    }
    
    return 0;
}