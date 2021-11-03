#include "Producer.h"
#include "Consumer.h"

int main(int argc, char* argv[])
{
    Producer producer(argc, argv);
    Consumer consumer(producer);
    consumer.start();
    return 0;
}

