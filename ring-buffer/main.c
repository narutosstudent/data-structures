#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // for malloc and free

typedef struct
{
	int *buffer;	   // Pointer to the storage of data items
	int capacity;	   // Max number of items the buffer can hold
	int size;	   // Number of items currently in the buffer
	int write_pointer; // Index where the next element will be written
	int read_pointer;  // Index from where the next element will be read
} RingBuffer;

RingBuffer *init_ringbuffer(int capacity)
{
	// 1. Allocate space for the RingBuffer structure.
	RingBuffer *rb = malloc(sizeof(RingBuffer));

	// 2. Check if the memory allocation succeeded.
	if (rb == NULL)
	{
		printf("Memory allocation failed.\n");
		exit(1);
	}

	// 3. Assign the 'capacity' to the RingBuffer's field.
	rb->capacity = capacity;

	// 4. Allocate memory for the data buffer.
	rb->buffer = malloc(capacity * sizeof(int));

	// 5. Check if the memory allocation for buffer succeeded.
	if (rb->buffer == NULL)
	{
		printf("Memory allocation failed.\n");
		exit(1);
	}

	// 6. Initialize buffer's metadata.
	rb->size = 0;
	rb->read_pointer = 0;
	rb->write_pointer = 0;

	// 7. Return the `rb` pointer.
	return rb;
}

bool write_ringbuffer(RingBuffer *rb, int data)
{
	// 1. Check if the buffer is full.
	// 2. If the buffer is full, return false.
	if (rb->size == rb->capacity)
	{
		return false;
	}

	// 3. Write the data.
	rb->buffer[rb->write_pointer] = data;

	// 4. Update the write_pointer.
	//    - Use the modulo operation with `rb->capacity` to ensure wrapping around.
	rb->write_pointer = (rb->write_pointer + 1) % rb->capacity;

	// 5. Increment the size.
	rb->size++;

	// 6. Return true, indicating data was added successfully.
	return true;
}

bool read_ringbuffer(RingBuffer *rb, int *data)
{
	// 1. Check if the buffer is empty.
	// 2. If the buffer is empty, return false.
	if (rb->size == 0)
	{
		return false;
	}

	// 3. Read data from the buffer.
	*data = rb->buffer[rb->read_pointer];

	// 4. Update the read_pointer.
	//    - Use the modulo operation with `rb->capacity` to ensure circuling back to the beginning.
	rb->read_pointer = (rb->read_pointer + 1) % rb->capacity;

	// 5. Decrement the size.
	rb->size--;

	// 6. Return true, indicating data was read successfully.
	return true;
}

void free_ringbuffer(RingBuffer *rb)
{
	// 1. Free the buffer's memory.
	free(rb->buffer);

	// 2. Free the memory occupied by the RingBuffer structure.
	free(rb);
}

int main()
{
	// Test the ring buffer

	// 1. Create a ring buffer with a capacity of 5
	RingBuffer *rb = init_ringbuffer(5);

	// 2. Write some values to it
	for (int i = 0; i < 5; i++)
	{
		if (!write_ringbuffer(rb, i + 1))
		{
			printf("Buffer full. Couldn't write %d\n", i + 1);
		}
	}

	// 3. Try writing when buffer is full
	if (!write_ringbuffer(rb, 6))
	{
		printf("Buffer full. Couldn't write 6\n");
	}

	// 4. Read values from the buffer
	int value;
	for (int i = 0; i < 5; i++)
	{
		if (read_ringbuffer(rb, &value))
		{
			printf("Read %d from buffer\n", value);
		}
		else
		{
			printf("Buffer empty\n");
		}
	}

	// 5. Try reading when buffer is empty
	if (!read_ringbuffer(rb, &value))
	{
		printf("Buffer empty. Couldn't read\n");
	}

	// 6. Free the ring buffer
	free_ringbuffer(rb);

	return 0;
}
