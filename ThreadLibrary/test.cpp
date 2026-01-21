#include <iostream>
#include "runnable.h"
using namespace std;

class Test : public runnable {
    private:
    
    public:
    bool stopRequested() const override {
        return false;
    }
    
    void run() override {
        cout << "Test" << endl;
    }
};



int main() {
    Test test;
    test.run();
    return 0;
}