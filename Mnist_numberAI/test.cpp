#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;


int main(){
    srand(time(0));
    float num = rand() %201;

    num = (num-100)/100;

    cout <<num<< "\n";

    return 0;
}