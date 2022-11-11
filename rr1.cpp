#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <queue>
#include <cstring>
using namespace std;
 
struct client {
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnTime;
    int waitingTime;
    int startTime;
};
 
int main() {
    int n,tq;
 
    cout << setprecision(2) << fixed;
    cout<<"Enter the number of clients: ";
    cin>>n;
    cout<<"Enter time quantum of CPU: ";
    cin>>tq;
    struct client p[n];
    float avgWaiting;
    int totalTurn = 0,burstArr[n];
    float avgTurn;
    int totalWaiting = 0,index;
    for(int i = 0; i < n; i++) {
        cout<<"Enter arrival time of client "<<i+1<<": ";
        cin>>p[i].arrivalTime;
        cout<<"Enter burst time of client "<<i+1<<": ";
        cin>>p[i].burstTime;
        burstArr[i] = p[i].burstTime;
        cout<<endl;
    }
    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark,0,sizeof(mark));
    mark[0] = 1;
 
    while(completed != n) {
        index = q.front();
        q.pop();
 
 
        if(burstArr[index] == p[index].burstTime) {
            p[index].startTime = max(current_time,p[index].arrivalTime);
            current_time = p[index].startTime;
        }
 
        if(0<burstArr[index]-tq) {
            burstArr[index] -= tq;
            cout<< "Process " << index << " is processed in time interval " << current_time <<" sec and ";
            current_time += tq;                                
            cout<< current_time <<" sec "<<endl;
        }
        else {
            cout<< "Process " << index << " is processed in time interval " << current_time <<" sec and ";
            current_time += burstArr[index];
            cout<< current_time <<" sec "<<endl;
            p[index].completionTime = current_time;
            p[index].turnTime = p[index].completionTime - p[index].arrivalTime;
            p[index].waitingTime = p[index].turnTime - p[index].burstTime;
            totalWaiting += p[index].waitingTime;
            totalTurn += p[index].turnTime;
            completed++;
            burstArr[index] = 0;
        }
        for(int i = 1; i < n; i++) {
            if(burstArr[i] > 0 && p[i].arrivalTime <= current_time && mark[i] == 0) {
                mark[i] = 1;
                q.push(i);
            }
        }
        if(0< burstArr[index]) q.push(index);
        if(q.empty()) {
            for(int i = 1; i < n; i++) {
                if(0<burstArr[i]) {
                    mark[i] = 1;
                    q.push(i);
                    break;
                }
            }
        }
    }
    avgWaiting = (float) totalWaiting / n;
    avgTurn = (float) totalTurn / n;
    cout<<endl;
    cout<<"Average Waiting Time = "<<avgWaiting<<endl;
    cout<<"Average Turnaround Time = "<<avgTurn<<endl;
    cout << "process_id "<<"arrivaltime "<<" completionTime" <<" waitingTime"<<" turnTime"<< endl;
    for (int  i = 0; i < n; i++)
    {
        cout <<i+1<<"         "<< p[i].arrivalTime<<"            "<<p[i].completionTime<<"            "<<p[i].waitingTime<<"            "<< p[i].turnTime<< endl;
    }
 
    return 0;
}
 