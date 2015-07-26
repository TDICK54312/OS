#include<iostream>
#include<sys/types.h>
#include<sstream>
#include<unistd.h>
#include<string>
#include<sys/ipc.h>
#include<stdio.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<list>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

struct Car{
    string plate;
    int secondsArrival;
    int carWeight;
    int secondsLeave;
};
/*int leave_bridge(int weight){

}*/
/*int enter_bridge(int weight){

}*/

int main(int args, char** argv){
    int i = 0;
    list<Car> carsList;
    int count = 0;
    string stuff[4];
    string line;
    int pid = -1;

    unsigned int MAX_WEIGHT = atoi(argv[1]);

    cout<<MAX_WEIGHT<<"\n";

    while(getline(cin,line)){
        stringstream ssin(line);
        Car x;

        while(ssin.good() && i < 4){
            ssin>>stuff[i];
            ++i;
        }
        x.plate = stuff[0];
        x.secondsArrival = atoi(stuff[1].c_str());
        x.carWeight = atoi(stuff[2].c_str());
        x.secondsLeave = atoi(stuff[3].c_str());
        carsList.push_back(x);
        if(count == 0){
            carsList.pop_front();
        }

        i = 0;
        count++;
    }
    //Just making sure the list was getting all the values it need *works*
    /*for(list<Car>::iterator it = carsList.begin(); it != carsList.end(); ++it){
        Car m = *it;

        cout<<"License Plate #: "<<m.plate<<" Enters at: "<<m.secondsArrival<<" Car weight: "<<m.carWeight<<" Leaving at: "<<m.secondsLeave<<endl;
    }*/
    
    for(list<Car>::iterator it = carsList.begin(); it != carsList.end(); ++it){
        //Car newCar = *it;

        pid = fork();
        if(pid == 0){
            Car newCar = *it;
            cout<<"Waiting "<<newCar.secondsArrival<<" seconds."<<endl;
            sleep(newCar.secondsArrival);
            
            

            break;

        }
        if(pid > 0)
        {
            wait(NULL);
        }


    }


    return 0;
}
