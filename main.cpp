//
//  main.cpp
//  IO-Scheduler
//
//  Created by Jessie on 28/04/2018.
//  Copyright © 2018 Jessie. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

struct Instruction{
    int arrivetime;
    int diskaddress;
    int insNum;
    int s_time;
    int e_time;
    bool operator < (const Instruction obj) const{
        return insNum < obj.insNum;
    }
};
bool CmpNum(Instruction i1, Instruction i2){
    return i1 < i2;
}
bool Com(Instruction i1, Instruction i2){
    if(i1.diskaddress == i2.diskaddress){
        return i1.insNum < i2.insNum;
    }
    else
    
    return i1.diskaddress < i2.diskaddress;
}


vector<Instruction> readyqueue;
vector<Instruction> printqueue;
vector<Instruction> switchqueue;
int sum = 10000;
bool direction = true;

int queueindex = 0;
int timestamp = 0;
int curr_track = 0;
int tot_movement = 0;
int tot_turnaround = 0;
int tot_waittime = 0;
int max_waittime = 0;

class IOScheduler{
    public:
    virtual Instruction get_next_IO(int curr_addr, bool direction, bool rempty) = 0;
};

class FIFO: public IOScheduler{
    public:
    Instruction get_next_IO(int curr_addr, bool direction, bool rempty) {
        
        Instruction instr = readyqueue.front();
        readyqueue.erase(readyqueue.begin());
        return instr;
    }
};

class SSTF: public IOScheduler{
    public:
    Instruction  get_next_IO(int curr_addr, bool direction, bool rempty){
        unsigned int min_dis = 65535;//Initialize the minimum distance to max int for unsigned int
        Instruction instr;
        int temp;
        int index = -1;
        for(int i = 0; i < readyqueue.size(); i++){
            temp = abs(readyqueue[i].diskaddress - curr_addr);
            if (temp < min_dis){
                min_dis = temp;
                index = i;
            }
        }
        instr = readyqueue[index];
        readyqueue.erase(readyqueue.begin()+index);
        return instr;
    }
};


class LOOK: public IOScheduler{
    public:
    Instruction get_next_IO(int curr_addr, bool direction, bool rempty){
        int index = -1;
        Instruction instr;
        instr.arrivetime = -1;
        instr.diskaddress = -1;
        instr.insNum = -1;
        instr.s_time = -1;
        instr.e_time = -1;
        int temp;
        unsigned int min_dis = 65535;
        
       if(direction){
           for(int i = 0; i < readyqueue.size(); i++){
               if(readyqueue[i].diskaddress >= curr_addr){
                   temp = abs(curr_addr -readyqueue[i].diskaddress);
                   if(temp < min_dis){
                       min_dis = temp;
                       index = i;
                   }
               }
            }
           
           if(index != -1){
            instr = readyqueue[index];
            readyqueue.erase(readyqueue.begin()+index);
           }
           
       }
        
        else if(!direction){
            for(int i = 0; i < readyqueue.size(); i++){
                if(curr_addr >= readyqueue[i].diskaddress){
                    temp = abs(curr_addr - readyqueue[i].diskaddress);
                    
                
                if(temp < min_dis){
                    min_dis = temp;
                    index = i;
                }
                }
            }
            
            if(index != -1){
            instr = readyqueue[index];
            readyqueue.erase(readyqueue.begin()+index);
            }
            
        }
     
        return instr;
    }
    
};



class CLOOK: public IOScheduler{
    public:
    Instruction get_next_IO(int curr_addr, bool direction, bool rempty){
        int index = -1;
        Instruction instr;
        instr.arrivetime = -1;
        instr.diskaddress = -1;
        instr.insNum = -1;
        instr.s_time = -1;
        instr.e_time = -1;
        int temp;
        int max_dis = -1;
        unsigned int min_dis = 65535;
        
      
            for(int i = 0; i < readyqueue.size(); i++){
                if(readyqueue[i].diskaddress >= curr_addr){
                    temp = abs(curr_addr -readyqueue[i].diskaddress);
                    if(temp < min_dis){
                        min_dis = temp;
                        index = i;
                    }
                }
            }
            
            if(index != -1){
                instr = readyqueue[index];
                readyqueue.erase(readyqueue.begin()+index);
            }
        if(index == -1){
       
            
            for(int i= 0; i< readyqueue.size(); i++){
                if(curr_addr >= readyqueue[i].diskaddress ){
                    temp =abs(curr_addr - readyqueue[i].diskaddress);
                    if(max_dis < temp){
                        max_dis = temp;
                         index = i;
                    }
                   
                }
             
            }
            
                instr = readyqueue[index];
                readyqueue.erase(readyqueue.begin()+index);
           
            }

        return instr;
    }
};



class FLOOK: public IOScheduler{
    public:
    Instruction get_next_IO(int curr_addr, bool direction, bool rempty){
        int index = -1;
        Instruction instr;
        instr.arrivetime = -1;
        instr.diskaddress = -1;
        instr.insNum = -1;
        instr.s_time = -1;
        instr.e_time = -1;
        
        int temp;
        unsigned int min_dis = 65535;
        if(!rempty){
        if(direction){
            for(int i = 0; i < readyqueue.size(); i++){
                if(readyqueue[i].diskaddress >= curr_addr){
                    temp = abs(curr_addr -readyqueue[i].diskaddress);
                    if(temp < min_dis){
                        min_dis = temp;
                        index = i;
                    }
                }
            }
            
            if(index != -1){
                instr = readyqueue[index];
                readyqueue.erase(readyqueue.begin()+index);
            }
            
        }
        
        else if(!direction){
            for(int i = 0; i < readyqueue.size(); i++){
                if(curr_addr >= readyqueue[i].diskaddress){
                    temp = abs(curr_addr - readyqueue[i].diskaddress);
                    if(temp < min_dis){
                        min_dis = temp;
                        index = i;
                    }
                }
            }
            
            if(index != -1){
                instr = readyqueue[index];
                readyqueue.erase(readyqueue.begin()+index);
            }
            
        }
            
    }
    
    if(rempty){
        if(direction){
        for(int i = 0; i < switchqueue.size(); i++){
            if(switchqueue[i].diskaddress >= curr_addr){
                temp = abs(curr_addr -switchqueue[i].diskaddress);
                if(temp < min_dis){
                    min_dis = temp;
                    index = i;
                }
            }
        }
        
        if(index != -1){
            instr = switchqueue[index];
            switchqueue.erase(switchqueue.begin()+index);
        }
        
    }
        
        else if(!direction){
            for(int i = 0; i < switchqueue.size(); i++){
                if(curr_addr >= switchqueue[i].diskaddress){
                    temp = abs(curr_addr - switchqueue[i].diskaddress);
                    
                    
                    if(temp < min_dis){
                        min_dis = temp;
                        index = i;
                    }
                }
            }
            
            if(index != -1){
                instr = switchqueue[index];
                switchqueue.erase(switchqueue.begin()+index);
            }
            
        }
        
        
    }
        return instr;
    }
    
};
    





    
int main(int argc, char * argv[]) {
    
    IOScheduler* sche = new FIFO();
    string algo;
    vector<Instruction> ins_queue;
    
    if (argc != 3){
        printf("Input File not found!");
        exit(1);
    }
    
    int c;
    opterr = 0;
    while ( (c = getopt(argc, argv, "s:")) != -1 )
    switch (c) {
        case 's':
        algo = string(optarg);
        break;
        case '?':
        if (optopt == 's')
        fprintf (stderr, "Option -%c requires an argument. \n", optopt);
        else if (isprint (optopt))
        fprintf (stderr, "Unknown option '-%c'.\n", optopt);
        default:
        abort ();
    }
    
    switch(algo[0]){
    
        case 'i':
            sche =new FIFO();
        break;
        case 'j':
            sche =new SSTF();
        break;
        case 's':
            sche =new LOOK();
        break;
        case 'c':
            sche =new CLOOK();
        break;
        case 'f':
            sche =new FLOOK();
        
        break;
    }
    
    
        ifstream ifile;
        string token;
        ifile.open(argv[2]);
        string line;
        int time;
        int address;
        int i = 0;
        
        while(getline(ifile, line)){
            
            if(line[0] == '#'){
                continue;
            }
            stringstream ss(line);
            ss >> time >> address;
            
            Instruction ins;
            
            
            ins.arrivetime = time;
            ins.diskaddress = address;
            ins.insNum = i;
            ins.s_time = 0;
            ins.e_time = 0;
            ins_queue.push_back(ins);
            
            i++;
        }
        
        ifile.close();
    
    sum = ins_queue.size();
    int count  = 0;
    bool direction = true;
    bool rempty = false;

    bool flook = false;
    if(algo[0]=='f'){
        flook = true;
    }
    
    while( count != sum) {
        
        if(readyqueue.empty()&&switchqueue.empty()){
            
            Instruction temp = ins_queue.front();
            ins_queue.erase(ins_queue.begin());
            readyqueue.push_back(temp);
            timestamp = temp.arrivetime;
            
            
            //cout << timestamp <<": "<< temp.insNum << " add "<<temp.diskaddress<< endl;
            
        }
        
        else if(!readyqueue.empty()||!switchqueue.empty()){
            count++;
            Instruction *instruc =  new Instruction();
            *instruc = sche->get_next_IO(curr_track,direction,rempty);
            if (instruc ->arrivetime == -1){
                if(!rempty&&!readyqueue.empty()){
                direction = !direction;
                }
                if(rempty&&!switchqueue.empty()){
                    direction = !direction;
                }
                *instruc = sche->get_next_IO(curr_track,direction,rempty);
                
                if(instruc ->arrivetime == -1){
                    rempty = !rempty;
                    *instruc = sche->get_next_IO(curr_track,direction,rempty);
                    if(instruc ->arrivetime == -1){
                        if(!rempty&&!readyqueue.empty()){
                            direction = !direction;
                        }
                        if(rempty&&!switchqueue.empty()){
                            direction = !direction;
                        }
                        *instruc = sche->get_next_IO(curr_track,direction,rempty);
                    }
                }
            }
            
                tot_waittime += timestamp - instruc->arrivetime;
                if(max_waittime < timestamp - instruc->arrivetime){
                    max_waittime = timestamp - instruc->arrivetime;
                }
            
            //cout << timestamp <<": "<< instruc->insNum << " issue "<<instruc->diskaddress<<" "<<curr_track<<endl;
            
            instruc->s_time = timestamp;
            instruc->e_time = timestamp+abs(instruc->diskaddress - curr_track);
            
            printqueue.push_back(*instruc);
            
            if(flook){
                if(!rempty){
                    while (!ins_queue.empty()&&ins_queue[0].arrivetime <= (timestamp+abs(instruc->diskaddress - curr_track))) {
                        Instruction temp = ins_queue[0];
                        ins_queue.erase(ins_queue.begin());
                        switchqueue.push_back(temp);
                        
                       // cout << temp.arrivetime <<": "<< temp.insNum << " add "<<temp.diskaddress<< endl;
                        
                    }
                    
                }
                if(rempty){
                    while (!ins_queue.empty()&&ins_queue[0].arrivetime <= (timestamp+abs(instruc->diskaddress - curr_track))) {
                        Instruction temp = ins_queue[0];
                        ins_queue.erase(ins_queue.begin());
                        readyqueue.push_back(temp);
                        
                      //  cout << temp.arrivetime <<": "<< temp.insNum << " add "<<temp.diskaddress<< endl;
                        
                    }
                    
                }
                
            }
            
            if(!flook){
            while (!ins_queue.empty()&&ins_queue[0].arrivetime <= (timestamp+abs(instruc->diskaddress - curr_track))) {
                Instruction temp = ins_queue[0];
                ins_queue.erase(ins_queue.begin());
                readyqueue.push_back(temp);

               // cout << temp.arrivetime <<": "<< temp.insNum << " add "<<temp.diskaddress<< endl;
                
            }
            }
        
            
          
            timestamp += abs(instruc->diskaddress - curr_track);
             //cout << timestamp <<": "<< instruc->insNum << " finish "<<abs(timestamp - instruc->arrivetime)<<endl;
            
            
            
            
            tot_turnaround += timestamp - instruc->arrivetime;
            tot_movement += abs(instruc->diskaddress - curr_track);
            curr_track = instruc->diskaddress;
            
        }
        
    }
    
    
    
    sort(printqueue.begin(),printqueue.end(),CmpNum);
    for(int i = 0; i < printqueue.size(); i++){
        printf("%5d: %5d %5d %5d\n", printqueue[i].insNum, printqueue[i].arrivetime, printqueue[i].s_time, printqueue[i].e_time);
    }
    
    double avg_turnaround = (double)tot_turnaround/sum;
    double avg_waittime = (double)tot_waittime/sum;
    printf("SUM: %d %d %.2lf %.2lf %d\n",
           timestamp, tot_movement, avg_turnaround, avg_waittime, max_waittime);
    
    
   
    
    return 0;
}

