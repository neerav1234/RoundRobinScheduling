#include<bits/stdc++.h>
#include<unistd.h>
#include<pthread.h>
#include<cstring>
#include<ctime>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<chrono>
using namespace std;
using std::chrono::milliseconds;
using std:: chrono ::duration_cast;
using std::chrono::system_clock;


#define PORT 3000

#define WIN_SIZE 20
#define QUANTA  4
#define GET_TIME duration_cast< milliseconds >(system_clock::now().time_since_epoch())


int64_t get_Time(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void log(string d){
    printf("[+]%s\n",d.c_str());
}
void logError(string d){
    printf("[!!]%s\n",d.c_str());
}

void check(int flag , string onSuccess , string onError){
    if(flag < 0)logError(onError);
    else log(onSuccess);
}

vector<int>queue_q(WIN_SIZE , -1);
vector<int>reqTime(WIN_SIZE , -1);
vector<string>cliAddress(WIN_SIZE);

int crnt_clients = 0;
pthread_mutex_t qLock = PTHREAD_MUTEX_INITIALIZER;


void* handleConnections(void* arg){
    log("Managing the clients");
    while (true)
    {
        
        for(int i = 0;i<WIN_SIZE;++i){

            pthread_mutex_lock(&qLock);
        
            int64_t start = get_Time();
            if(queue_q[i] == -1){
                pthread_mutex_unlock(&qLock);
                continue;
            }
            int fd = queue_q[i];

            if(reqTime[i]==-1){
                string hello = "Hell0 !! What is your reqTime ?";
                send(queue_q[i], hello.c_str(), strlen(hello.c_str()), 0);
                recv(fd , &reqTime[i],sizeof(reqTime[i]),0);
                reqTime[i] = ntohl(reqTime[i]);
                log("ReqTime of new client : "+ to_string(reqTime[i]));
            }

            for(;reqTime[i] >=0;--reqTime[i]){
                int64_t cpu_time = (get_Time()-start);
                cout<<cpu_time<<"\n";
                if(cpu_time >=QUANTA){
                    log("Client " + to_string(i) + " timeout");
                    break;
                }
                string hello = ("reqTime left : " + to_string(reqTime[i]));
                send(queue_q[i], hello.c_str(), strlen(hello.c_str()), 0);
                log("msg sent to Client "+ to_string(i) + "  " + hello);
            }


            if(reqTime[i] == -1){
                close(queue_q[i]);
                queue_q[i] = -1;
                crnt_clients--;
            }
            pthread_mutex_unlock(&qLock);
            
        }
    }
    
    
    return NULL;
}

void clientAccecptor(int server_fd){
    
    
    while(true){
        log("Can accept clients....");
        int clientFd;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

        check(clientFd = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen) , "new client accepted","accept failure");
        

    
        pthread_mutex_lock(&qLock);
        if(crnt_clients == WIN_SIZE){
            
            string msg = "Server Queue full :( Try again later.";
            send(clientFd, msg.c_str(), msg.length(), 0);
            close(clientFd);
            pthread_mutex_unlock(&qLock);
            continue;
        }
        
        
        int free_idx = -1;
        for(int i = 0;i<WIN_SIZE;++i){
            if(queue_q[i] == -1){
                free_idx = i;
                break;
            }
        }

        queue_q[free_idx] = clientFd;
        crnt_clients++;
        log("free idx " + to_string(free_idx));
        pthread_mutex_unlock(&qLock);
        
        
    }
}

int main(int argc , char** argv){
    log("inside main");
    
    pthread_t tid;
    pthread_create(&tid,NULL,&handleConnections,NULL);
    
    int server_fd;
	struct sockaddr_in address;
	int opt = 1;
    
    check(server_fd = socket(AF_INET , SOCK_STREAM , 0 ) , "fd generated","socket fail");
    check(setsockopt(server_fd, SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT, &opt,
            sizeof(opt)),"socket opts","sockopt error");

    address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

    check(bind(server_fd, (struct sockaddr*)&address,
			sizeof(address)),"socket binded","binding failure");
    check(listen(server_fd , 3),"Server listening...." , "listen failed");


    
    clientAccecptor(server_fd);
    pthread_join(tid , NULL);
}