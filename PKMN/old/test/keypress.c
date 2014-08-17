#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/keyboard.h>
#include <linux/kd.h>

int is_keyboard(int fd)
{
	int data;

	data = 0;
	if(ioctl(fd, KDGKBTYPE, &data) != 0)
	{
		return 0;
	}

	if(data == KB_84)
	{
		printf("84-key keyboard found.\n");
		return 1;
	}
	else if(data == KB_101)
	{
		printf("101-key keyboard found.\n");
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	struct termios old_term, new_term;
	int kb = -1;
	char *files_to_try[] = {"/dev/tty", "/dev/console", NULL};
	int old_mode = -1;
	int i;

	for(i = 0 ; files_to_try[i] != NULL ; ++i)
	{
		kb = open(files_to_try[i], O_RDONLY);
		if(kb < 0) continue;

		if(is_keyboard(kb))
		{
			printf("Using keyboard %s.\n", files_to_try[i]);
			break;
		}

		close(kb);
	}

	if(files_to_try[i] == NULL)
	{
		for(kb = 0 ; kb < 3 ; ++kb)
		{
			if(is_keyboard(kb)) break;
		}

		if(kb == 3)
		{
			printf("Unable to find a file descriptor associated "
					"with the keyboard.\n"
					"Perhaps this isn't a virtual terminal?\n");

			return EXIT_FAILURE;
		}
	}
	
	if(ioctl(kb, KDGKBMODE, &old_mode) != 0)
	{
		printf("Unable to query keyboard mode.\n");
		goto error;
	}

	if(tcgetattr(kb, &old_term) != 0)
	{
		printf("Unable to query terminal settings.\n");
		goto error;
	}

	new_term = old_term;
	new_term.c_iflag = 0;
	new_term.c_lflag &= ~(ECHO | ICANON | ISIG);

	if(tcsetattr(kb, TCSAFLUSH, &new_term) != 0)
	{
		printf("Unable to change terminal settings.\n");
		goto error;
	}

	if(ioctl(kb, KDSKBMODE, K_MEDIUMRAW) != 0)
	{
		printf("Unable to set mediumraw mode.\n");
		goto error;
	}

	printf("Reading keycodes. Press escape to exit.\n");

	for(;;)
	{
		unsigned char data;

		if(read(kb, &data, 1) < 1)
		{
			printf("Unable to read data.\n");
			goto error;
		}

		printf("%.8s: 0x%02X (%d)\n",
			(data & 0x80) ? "Released" : "Pressed",
			(unsigned int) data & 0x7f,
			(unsigned int) data & 0x7f);

			if((data & 0x7f) == 1)
			{
				printf("Escape pressed.\nExit.\n");
				break;
			}
	}

	ioctl(kb, KDSKBMODE, old_mode);
	tcsetattr(kb, 0, &old_term);

	if(kb >= 3) close(kb);

	return EXIT_SUCCESS;


error:
	
	printf("Cleanup:\n");

	fflush(stdout);

	if(old_mode != -1)
	{
		ioctl(kb, KDSKBMODE, old_mode);
		tcsetattr(kb, 0, &old_term);
	}

	if(kb >= 3) close(kb);

	return EXIT_FAILURE;
}

