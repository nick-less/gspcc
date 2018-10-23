#include <syscalls.h>
#include <task.h>

int nswitches;
int startTime;

task task1(4, "TASK1"), task2(4, "TASK2");

void task1Code(long arg0, ...)
{
	for ( ; ; )
	{
		message_t payload;

		sendMessage(&task2, 1);
		payload = getMessage();
	}
}

void task2Code(long arg0, ...)
{
	for ( ; ; )
	{
		message_t payload;

		sendMessage(&task1, 2);
		payload = getMessage();
		nswitches++;
		if (nswitches > 1000)
		{
			int stopTime = GET_TICKS();

			PRINTF("%d switches in %d ticks\n",
						nswitches, stopTime - startTime);
			PRINTF("%d switches per second\n",
					nswitches / ((stopTime - startTime) * 50));
			EXIT(0);
		}
	}
}

int main(int argc, char *argv[])
{
	createTask(task1Code, &task1, 0, 2, 3, 4);
	createTask(task2Code, &task2, 0, 3, 4, 5, 6);
	nswitches = 0;
	startTime = GET_TICKS();
	nextTask();

	PRINTF("PANIC\n");
}

