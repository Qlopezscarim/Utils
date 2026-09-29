#include <vector>
#include <atomic>
#include "OS_ABSTRACTION.h"

class QFifo 
{
public:
	struct MetaData
	{

		std::atomic<int> associated_state = -1;
		void*       data;

	};

	MetaData Buffer[QFIFO_SIZE];
	MetaData* metadata_start = Buffer;
	std::vector<MetaData*> state_pointers;

QFifo(int number_of_states);
MetaData* get_new_metadata(int associated_state);
void next_state(int associated_state);
};
