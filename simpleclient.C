#include <cassert>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <errno.h>

#include "reqchannel.H"
#include "semaphore.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/
void print_time_diff(struct timeval * tp1, struct timeval * tp2) {
  /* Prints to stdout the difference, in seconds and museconds, between two
     timevals. */

  long sec = tp2->tv_sec - tp1->tv_sec;
  long musec = tp2->tv_usec - tp1->tv_usec;
  if (musec < 0) {
    musec += 1000000;
    sec--;
  }
  printf(" [sec = %ld, musec = %ld] ", sec, musec);

}
class Item
{
private:
    int req_id;
    string req_data;
    
public:
    Item(string _req_data)
    {
        
        req_data = _req_data;
		req_id = 0;
    };
    Item(){};

	~Item(){};

	string get_data() { return req_data; }
	int get_id() { return req_id; }
void set_data(string _data) {req_data = _data;}
	void set_id(int id) {req_id = id;}
	
};


class BoundedBuffer 
{
	private:
	int msize;
		Semaphore * lock;
		Semaphore * full;
		Semaphore * empty;

		
		

	public:

	queue<Item> bb;

		BoundedBuffer(int _size)
		{
			msize = _size;
			lock = new Semaphore(1);
			full = new Semaphore(0);
			empty = new Semaphore(_size);
			
		}
		BoundedBuffer(){}
		
		void deposit(Item _item)
		{
			empty->p();
			lock->p();
			bb.push(_item);
			lock->v();
			full->v();
		   
		}

		Item  remove()
		{
			full->p();
			lock->p();

			Item temp;			
			temp = bb.front();	// get item from queue
			bb.pop();						// delete item from queue

			lock->v();
			empty->v();

			return temp;

		}
		
		void setsize(int size) { msize = size; };
		int size() {return bb.size(); };
		
 };

 
BoundedBuffer buff;
BoundedBuffer joe_b;
BoundedBuffer jane_b;
BoundedBuffer john_b;

int opt,datareq,buffsize,worker;

pthread_t joes_thread;
pthread_t janes_thread;
pthread_t johns_thread;

pthread_t stat_thread_1;
pthread_t stat_thread_2;
pthread_t stat_thread_3;
	struct timeval tp_start; 
	struct timeval tp_end;
int joes_stats[100];
int janes_stats[100];
int johns_stats[100];

 
void * send_Requests(void * thread)
{	 
	cout<< "in send requests \n ";
	int person= *(int*)thread; 
	//Item* temp = new Item;
	Item temp;
	for(int i = 0; i < datareq; i++)
    {        
		if(person==1){
			temp.set_data("data Joe Smith");
			temp.set_id(1);
			buff.deposit(temp);
			//cout<< " deposited \n";
			}
		if(person==2){
			temp.set_data("data Jane Smith");
			temp.set_id(2);
			buff.deposit(temp);
			//buff.deposit("data Jane Smith");
			//cout<< " deposited \n";
			}
		if(person==3){
			temp.set_data("data John Doe");
			temp.set_id(3);
			buff.deposit(temp);
			//buff.deposit("data John Doe");
			//cout<< " deposited \n";
			}
	}	
      
    
     
	pthread_exit(NULL);

}

void create_request()
{

		void* joe1 = new int(1);
		void* joe2 = new int(2);
		void* joe3 = new int(3);
		
		string  thread3= "data John Doe" ;
		string   thread2= "data Jane Smith" ;
			string  thread= "data Joe Smith" ;
		//cout<< " pre joe pthread \n ";
			pthread_create(&joes_thread, NULL, send_Requests, joe1);
		// cout<< " post joe pthread \n";

		 
			pthread_create(&janes_thread, NULL, send_Requests, joe2);
			
	//	cout<< " pre jane pthread \n";
 
            pthread_create(&johns_thread, NULL, send_Requests,joe3);
	//	cout<< " post john pthread \n";


	 

}
void * worker_requests(void * channel)
{
	 
	string quit_string = "";
	RequestChannel * worker_channel[worker];
	
	fd_set my_set;
	int readfs[worker];
	int writefs[worker];
	int ids[worker];
	int retval;
	int max=0;
	struct timeval tv;
<<<<<<< HEAD
<<<<<<< HEAD
	Item request;
	int id;
=======

>>>>>>> parent of 655ee20... working no bonus
=======

>>>>>>> parent of 655ee20... working no bonus
	
	for(int i =0;i<worker;i++)
	{	
	
		RequestChannel channel_name("control", RequestChannel::CLIENT_SIDE);
		RequestChannel* workers_channel = new RequestChannel(channel_name, RequestChannel::CLIENT_SIDE);
		
<<<<<<< HEAD
<<<<<<< HEAD
		readfs[i]=workers_channel->read_fd();
		//printf(" %s string \n",workers_channel->read_fd());
		writefs[i]=workers_channel->write_fd();
		worker_channel[i]= workers_channel;
		if(readfs[i] >max)
			max=readfs[i];
		if(buff.size()>0)
		{		
			request = buff.remove();
			string tempr=request.get_data();
			id = request.get_id();
			worker_channel[i]->cwrite(request.get_data());
			//write(writefs[i],tempr.c_str(),strlen(tempr.c_str()+1));
			ids[i]=id;
			chan_counter++;
		}
=======
		readfs[i]=worker_channel.read_fd();
		writefs[i]=worker_channel.write_fd();
		worker_channel[i]= workers_channel;
		if(readfs[i] >max)
			max=readfs[i];
=======
		readfs[i]=worker_channel.read_fd();
		writefs[i]=worker_channel.write_fd();
		worker_channel[i]= workers_channel;
		if(readfs[i] >max)
			max=readfs[i];
>>>>>>> parent of 655ee20... working no bonus
		
		


<<<<<<< HEAD
>>>>>>> parent of 655ee20... working no bonus
=======
>>>>>>> parent of 655ee20... working no bonus
	}
	FD_ZERO(&my_set);
	for(int i =0;i<worker;i++)
	{	
		
		FD_SET(&my_set,readfs[i]);
	
	}
<<<<<<< HEAD
<<<<<<< HEAD
 
	while(1) {

		retval = select(max+1, &my_set, NULL, NULL, NULL);
		if(retval>0){
			for(int i=0;i<worker;i++)
			{
				if(FD_ISSET(readfs[i],&my_set))
				{	
					chan_counter--;
					char read_b2[255];
					char read_b[255];
					read(readfs[i], read_b2, 255);
					string channel_reply=worker_channel[i]->cread();
					printf(" channel reply %d \n ", channel_reply);
					//set_data(string _data)
					Item newreply;
					
					newreply.set_data(channel_reply);
					//string reply_to_request = (*workers_channel).send_request(channel_reply);
					read(writefs[i],read_b,255);
				//  printf("200 string %s 2 \n\n \n", read_b2);
				
					// printf("100 string %s 1 \n\n \n", read_b);

					
=======
	
	for(int i=0;i<worker;i++)
	{
		if(FD_ISSET(readfs[i],&my_set))
		{
			Item request;
			char read_b[255];
			
			if(buff.size() > 0)
			{
=======
	
	for(int i=0;i<worker;i++)
	{
		if(FD_ISSET(readfs[i],&my_set))
		{
			Item request;
			char read_b[255];
			
			if(buff.size() > 0)
			{
>>>>>>> parent of 655ee20... working no bonus
				retval = select(max+1, &readfs, &writefs, NULL, &tv);
				if(retval!=1){
					 request = buff.remove();
					 int id = request.get_id();
					 string tempr=request.get_data();
					// string reply; 
					//string reply_to_request = (*worker_channel).send_request(tempr);
					//request.set_data(reply_to_request);
<<<<<<< HEAD
>>>>>>> parent of 655ee20... working no bonus
=======
>>>>>>> parent of 655ee20... working no bonus
					 
						int cwrite(string tempr);
						string reply_to_request=(*worker_channel).cread();
						
<<<<<<< HEAD
<<<<<<< HEAD
						request = buff.remove();
						string tempr=request.get_data();
						worker_channel[i]->cwrite(request.get_data());	
						chan_counter++;		
						id=request.get_id();
						if(id==3)
					{
						 
						john_b.deposit(request);
					}
					if( id==2)
					{
					newreply.set_id(2);
						jane_b.deposit(request);
					}
							 
					if(id==1) 
					{
					newreply.set_id(1);
						joe_b.deposit(request);
					}
					}

				} 
				
=======
=======
>>>>>>> parent of 655ee20... working no bonus
					 
					cout<< " ID IS " << id << endl;
					if(id==1)
						joe_b.deposit(request);
					if(id==2)
						jane_b.deposit(request);
					if(id==3)
						john_b.deposit(request);

					int descriptor= select(/*everything*/) 
				}
			else{
				printf("Error calling select.");
				exit(1);
				}
<<<<<<< HEAD
>>>>>>> parent of 655ee20... working no bonus
			}
		}
<<<<<<< HEAD

		if(buff.size()==0 && chan_counter==0)
			{
			 usleep(100000);
			break;
		}
		
	}
			    for(int i=0;i<worker;i++)
	quit_string = worker_channel[i]->send_request("quit");	

	
 
    
=======
    }
    quit_string = worker_channel->send_request("quit");
   // usleep(1000000);
>>>>>>> parent of 655ee20... working no bonus
=======
			}
		}
    }
    quit_string = worker_channel->send_request("quit");
   // usleep(1000000);
>>>>>>> parent of 655ee20... working no bonus

}


void *create_statistics(void * nothing)
{
	//while(buff.size()>0)
	{
	
//int idx = atoi((stat_item->get_data()).c_str());
		while(joe_b.size() > 0)
		{
			//while 
			Item item; 
			item = joe_b.remove();
			int result =atoi(item.get_data().c_str());
			cout<< "result is " << result << endl;
			if(result-1<100)
			{
				joes_stats[result-1]++;
			}
			 
		}
		cout<< " JANE IS SIZE " << jane_b.size() << endl;
		while(jane_b.size() > 0)
		{
			Item item; 
			item = jane_b.remove();
			int result =atoi(item.get_data().c_str());
			cout<< "result is " << result << endl;
			if(result-1<100)
			{
				janes_stats[result-1]++;
			}
			 
		}	
		while(john_b.size() > 0)
		{
			Item item; 
			item = john_b.remove();
			int result =atoi(item.get_data().c_str());
			cout<< "result is " << result << endl;
			if(result-1<100)
			{
				johns_stats[result-1]++;
			}
			 
		} 
	}

	pthread_exit(NULL);

}

void generate_stat_threads()
{

	pthread_create(&stat_thread_1, NULL, create_statistics, NULL);
	pthread_create(&stat_thread_2, NULL, create_statistics, NULL);
	pthread_create(&stat_thread_3, NULL, create_statistics, NULL);
	usleep(1000000);
}

void print_stats(string name, int * stat_bucket)
{
 
	int total = 0;

    cout << "\n ";
	 
	cout << "Stat  for " << name << "  :  " << endl;
    cout << "--------------------------------------------------------------" << endl;
	for(int i = 0; i < 101; i++)
	{
		if(i % 10 == 0 & i !=0)
		{
            if(i != 100)
                total += stat_bucket[i];
            
			cout << "Returned Value:\t" << i - 10 << " to\t" << i - 1 << "\t" << total << " times "
				 << "\t \t (" << (((double)total)/((double)datareq))*100  << "%)" << endl;

            total = 0;
		}

		else
			total += stat_bucket[i];

	}
	
    cout << "--------------------------------------------------------------" << endl;
    cout << endl;


}

 
int main(int argc, char * argv[]) 
{
	int childid = fork();		
	if(childid == 0) {
		execl("dataserver",NULL);
	}
	else {
	assert(gettimeofday(&tp_start, 0) == 0); //start
		int errcode;
		cout << "Establishing control channel... " << flush;
		RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
		cout << "dones." << endl;;
		while((opt= getopt(argc, argv, "n:b:w:"))!= -1)
		{
			switch(opt)
			{
				case 'n':
				datareq=atoi(optarg);
				break;
				case 'b':
				buffsize=atoi(optarg);
				break;
				case 'w':
				worker=atoi(optarg);
				break;
			}
		}
	

		
		// stat_size = 3 people with data_requests_per_person
		int stat_size = 3 * datareq;

cout<<" setting bound buff \n";
		// initialize the buffers 
		buff = BoundedBuffer(buffsize);
		joe_b = BoundedBuffer(stat_size);
		jane_b= BoundedBuffer(stat_size);
		john_b= BoundedBuffer(stat_size);

		pthread_t workers_thread;
		// initialize the threads 
		//pthread_t workers[worker];
		cout<< " created workers \n";

			create_request();
		  cout<<" generated requests \n ";
 
<<<<<<< HEAD
			pthread_create(&workers_thread , NULL, worker_requests, chan);
			
	
			 

			  usleep(10000);  
			
		//	generate_stat_threads();
	//	usleep(1000000);
=======
			   
			}
			*/
			pthread_create(&workers_thread , NULL, worker_requests, NULL);
			
			//work
  	usleep(1000000);
			for(int i = 0; i < worker; i++)
			{
				
				pthread_join(workers[i], NULL);
			   
			}
			 
			generate_stat_threads();
	usleep(1000000);
>>>>>>> parent of 655ee20... working no bonus
			
			 
			//print_stats("Joe Smith", joes_stats);
			//print_stats("Jane Smith", janes_stats);
			//print_stats("John Doe", johns_stats); 
			//cout<< " JANE IS SIZE " << jane_b.size() << endl;
			usleep(1000000);
		
			 
		   string quit_string = chan.send_request("quit");
		   
		 		assert(gettimeofday(&tp_end, 0) == 0); //End latency measurement
		printf("Time taken for computation : "); 
		print_time_diff(&tp_start, &tp_end);
		printf("\n");
		
	}

	
	







	return 0;
  
}