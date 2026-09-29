#include "QFifo.h"

QFifo::QFifo(int number_of_states)
{
	for (int i=0; i<number_of_states; i++)
	{
		// Create a vector of length corresponding to number of stages in RX/TX Chain
		// Push the start of metadata array ; metadata_start is always pointing to start
		state_pointers.push_back(metadata_start);
	}
}

QFifo::MetaData* QFifo::get_new_metadata(int associated_state)
{
	//Cases to consider here : is the next slot meant for us? (Check the associated state!)
	//We are also okay to claim if uninitialized and we are the first state (Second part of or statement, unclaimed is -1)
	MetaData* next_metadata = (state_pointers[associated_state]);
	if(next_metadata->associated_state == associated_state || (next_metadata->associated_state == -1 && associated_state == 0) )
	{
		//std::cout << "Initializing" << std::endl;
		return (state_pointers[associated_state]);
	}
	else //(next_metadata.associated_state != associated_state )
	{
		if(associated_state == 0)
		{
			//This is an actual data loss! we can't push new data in at a sufficient rate
			return (state_pointers[associated_state]);

			//High Level Logging Error
			Os_Abstraction::QLOG_INFO("DATA LOSS IN FIFO");
		}
		else
		{

			//We are pending on data!
			//pend until the associated state is this one!
			int observed_slot_state = next_metadata->associated_state.load();
			while( observed_slot_state != associated_state )
			{
				//High Level Logging Error
                        	Os_Abstraction::QLOG_INFO("WARNING: FIFO CONTENTION FOR STATE " + std::to_string(associated_state) + " SLOT ASSIGNED " + std::to_string(next_metadata->associated_state));
				next_metadata->associated_state.wait(observed_slot_state);
				observed_slot_state = next_metadata->associated_state.load();
			}
			return (state_pointers[associated_state]);
		}
	}
}

void QFifo::next_state(int associated_state)
{
	//We want to unclaim metadata if on the last state:
	//Reminder that -1 is unclaimed
	if(associated_state == state_pointers.size()-1)
	{
		state_pointers[associated_state]->associated_state = -1;
	}
	else
	{
		state_pointers[associated_state]->associated_state = associated_state + 1;
	}
	//Notify any thread pending on this associated state to wake up
	state_pointers[associated_state]->associated_state.notify_all();


	//Should we just incriment the pointer, or are we past the end of the queue and need to loop around (Circular Buffer)
	if( ((state_pointers[associated_state] + 1)-metadata_start) < (QFIFO_SIZE) )
	{
		state_pointers[associated_state] = state_pointers[associated_state] + 1;
	}
	else
	{
		state_pointers[associated_state] = metadata_start;
	}



}
