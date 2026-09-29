#include "QFifo.h"
#include <iostream>

const static int num_iterations = 10000;
int test_buff[num_iterations]; 

void check_fifo_overflow()
{
	bool passed = true;
	//technically you could get insanely lucky and all initialized memory happens to be the right number
	QFifo Fifo_1 = QFifo(3);
	//int num_iterations = 10000;
	//int test_buff[num_iterations]; 
	for (int i=0 ; i<num_iterations; i++)
	{
		//Task 1: Load Task
		QFifo::MetaData* state_0_meta = Fifo_1.get_new_metadata(0);
		state_0_meta->data = test_buff + i;
		Fifo_1.next_state(0);
		
		//Task 2: Compute/Store
		QFifo::MetaData* state_1_meta = Fifo_1.get_new_metadata(1);
		int* data_pointer_1 = static_cast<int*>(state_1_meta->data);
		*data_pointer_1 = 99;
		Fifo_1.next_state(1);

		//Verify
		QFifo::MetaData* state_2_meta = Fifo_1.get_new_metadata(2);
		int* data_pointer_2 = static_cast<int*>(state_2_meta->data);
		int check_value = *data_pointer_2;
		if(check_value != 99)
		{
			passed = false;
		}
		Fifo_1.next_state(2);
	}
	if(passed)
	{
		std::cout << "Test 1 for QFifo Passed!" << std::endl;
	}
	else
	{
		std::cout << "Test 1 for QFifo FAILED! *****************" << std::endl;
	}
}

void Test_Fifo_Load_Task(QFifo& Fifo)
{
	//Generate fake data, but in most cases this would be real data pointers passed from UHD!
	for (int i=0 ; i<num_iterations; i++)
        {
                //Task 1: Load Task
                QFifo::MetaData* state_0_meta = Fifo.get_new_metadata(0);
                state_0_meta->data = test_buff + i;
                Fifo.next_state(0);
		Os_Abstraction::QSLEEP();
		Os_Abstraction::QLOG_INFO(std::to_string(i));
	}
	std::cout << "Load thread exited normally" << std::endl;


}

void Test_Fifo_Compute_Task(QFifo& Fifo)
{

        for (int i=0 ; i<num_iterations; i++)
        {
                //Task 2: Compute/Store
                QFifo::MetaData* state_1_meta = Fifo.get_new_metadata(1);
                int* data_pointer_1 = static_cast<int*>(state_1_meta->data);
                *data_pointer_1 = 99;
                Fifo.next_state(1);
		Os_Abstraction::QSLEEP();
	}
	std::cout << "Compute thread exited normally" << std::endl;
}

void Test_Fifo_Store_Task(QFifo& Fifo)
{
	bool passed = true;
        //technically you could get insanely lucky and all initialized memory happens to be the right number
        for (int i=0 ; i<num_iterations; i++)
        {


		//Verify
                QFifo::MetaData* state_2_meta = Fifo.get_new_metadata(2);
                int* data_pointer_2 = static_cast<int*>(state_2_meta->data);
                int check_value = *data_pointer_2;
                if(check_value != 99)
                {
                        passed = false;
                }
                Fifo.next_state(2);
	}

	if(passed)
        {
                std::cout << "Test 2 (Multithreaded contention check) for QFifo Passed!" << std::endl;
        }
        else
        {
                std::cout << "Test 2 (Multithreaded contention check) for QFifo FAILED! *****************" << std::endl;
        }

	std::cout << "Store thread exited normally" << std::endl;

}

int main()
{
	//Test 1
	check_fifo_overflow();

	//Test 2
	{
	//Test contention
	QFifo test_2_fifo = QFifo(3);
	std::thread load_thread     (Test_Fifo_Load_Task,   std::ref(test_2_fifo));
	std::thread compute_thread  (Test_Fifo_Compute_Task,std::ref(test_2_fifo));
	std::thread store_thread    (Test_Fifo_Store_Task,  std::ref(test_2_fifo));
	
	//Join all threads
	load_thread.join();
	compute_thread.join();
	store_thread.join();
	}
	std::cout << "Success" << std::endl;
}
