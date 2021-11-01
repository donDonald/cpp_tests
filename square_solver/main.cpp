#include "Producer.h"
#include "Consumer.h"

int main(int argc, char* argv[])
{
    InputQueue queue;
    Producer producer(queue, argc, argv);
    Consumer consumer(queue);
    consumer.start();
    return 0;
}

