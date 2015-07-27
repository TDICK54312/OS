#include<iostream>
#include<sys/shm.h>
#include<sys/sem.h>
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
    int i = 0;                          //used in parser
    list<Car> carsList;                 //creates a list of cars to be accessed FCFS
    int count = 0;                      //used to see how many carsare in the list
    string stuff[4];                    //used in parser 
    string line;                        //used for parser
    int pid = -1;                       //initialize pid
    int *MAX_WEIGHT; //= atoi(argv[1]); //shared variable
    key_t shmkey;                       //shared memory key
    int shmid;                          //shared memory id
    int value;                          //semaphore value to check if 1 or 0
    //size_t sizechar = NBYTES;           //size of shared memory
    sem_t *mutex;                        //synch semaphore 

    /* initialize shared variable in shared memory */
    shmid = shmget(shmkey, sizeof(int), 0600 | IPC_CREAT);

    /* shared memory error check */
    if(shmid < 0){
        perror("shmget\n");
        exit(1);
    }

    /* makes sure there are two arguments */
    if(args != 2){
        perror("Wrong number of arguments.");
        exit(1);
    }
    
    /* Attatch MAX_WEIGHT to shared memory */
    MAX_WEIGHT = (int *) shmat (shmid, NULL, 0);
    *MAX_WEIGHT = atoi(argv[1]);
    cout<<"MAX_WEIGHT on bridge "<<*MAX_WEIGHT<< " is allocated in shared memory."<<endl;


    //Used to make sure i was getting the max weight
    //cout<<MAX_WEIGHT<<"\n";
    
    /* This gets the input from input1.txt */
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

        if(x.carWeight > *MAX_WEIGHT)
        {
            perror("You entered an invalid weight. Please change text file.");
            _exit(1);
        }

        i = 0;
        count++;
    }
    
    /* Create Semaphore */
    mutex = sem_open("TD", O_CREAT, 0600, 1);
    /* Unlink prevents the semaphore from existing forever */
    sem_unlink("TD");
    
    /* Forking process */
    for(list<Car>::iterator it = carsList.begin(); it != carsList.end(); ++it){
        
        pid = fork();
        if(pid == 0){
            Car newCar = *it;

            /* Print arrival of car */
            sleep(newCar.secondsArrival);
            cout<<"Car: "<<newCar.plate<<" arrives."<<"     BridgeLoad Available: "<<*MAX_WEIGHT<<endl;
            int s = sem_getvalue(*mutex, &value);
            if(s == -1)
            {
                perror("error get value from semaphore.");
                _exit(1);
            }
            if(value == 1){
                if(*MAX_WEIGHT - newCar.carWeight >= 0){
                    *MAX_WEIGHT = *MAX_WEIGHT - newCar.carWeight;
                    cout<<"Car: "<<newCar.plate<<" enters bridge."<<"   BridgeLoad Available: "<<*MAX_WEIGHT<<endl;
                    sleep(newCar.secondsLeave);
                    *MAX_WEIGHT = *MAX_WEIGHT + newCar.carWeight;
                    cout<<"Car: "<<newCar.plate<<" leaves bridge."<<"   BridgeLoade Available: "<<*MAX_WEIGHT<<endl;
                }
                if(*MAX_WEIGHT - newCar.carWeight < 0){
                    sem_wait(mutex);
                    bool x = true;
                    while(x == true){
                        if(*MAX_WEIGHT - newCar.carWeight >= 0){
                            x = false;
                        }
                    }
                    *MAX_WEIGHT = *MAX_WEIGHT - newCar.carWeight;
                    cout<<"Car: "<<newCar.plate<<" enters bridge."<<"   BridgeLoad Available: "<<*MAX_WEIGHT<<endl;
                    sleep(newCar.secondsLeave);
                    *MAX_WEIGHT = *MAX_WEIGHT + newCar.carWeight;
                    cout<<"Car: "<<newCar.plate<<" leaves bridge."<<"   BridgeLoade Available: "<<*MAX_WEIGHT<<endl;
                    sem_post(mutex);
                }
            }
            if(value == 0)
            {
                bool y = true;

                while(y == true){
                    if(*MAX_WEIGHT - newCar.carWeight >= 0){
                        y = false;
                    }
                }
                if(value == 1){
                    if(*MAX_WEIGHT - newCar.carWeight >= 0){
                        *MAX_WEIGHT = *MAX_WEIGHT - newCar.carWeight;
                        cout<<"Car: "<<newCar.plate<<" enters bridge."<<"   BridgeLoad Available: "<<*MAX_WEIGHT<<endl;
                        sleep(newCar.secondsLeave);
                        *MAX_WEIGHT = *MAX_WEIGHT + newCar.carWeight;
                        cout<<"Car: "<<newCar.plate<<" leaves bridge."<<"   BridgeLoade Available: "<<*MAX_WEIGHT<<endl;
                    }
                    if(*MAX_WEIGHT - newCar.carWeight < 0){
                        sem_wait(mutex);
                        bool a = true;
                        while(a == true){
                            if(*MAX_WEIGHT - newCar.carWeight >= 0){
                                a = false;
                            }
                        }
                        *MAX_WEIGHT = *MAX_WEIGHT - newCar.carWeight;
                        cout<<"Car: "<<newCar.plate<<" enters bridge."<<"   BridgeLoad Available: "<<*MAX_WEIGHT<<endl;
                        sleep(newCar.secondsLeave);
                        *MAX_WEIGHT = *MAX_WEIGHT + newCar.carWeight;
                        cout<<"Car: "<<newCar.plate<<" leaves bridge."<<"   BridgeLoade Available: "<<*MAX_WEIGHT<<endl;
                        sem_post(mutex);
                }
              }
            }

            break;

        }
        if(pid > 0)
        {
            wait(NULL);
        }


    }


    return 0;
}
