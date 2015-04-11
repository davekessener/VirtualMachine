#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "io.h"

int main(int argc, char *argv[])
{
	uint count = 0;
	int state = 0;
	int fwd = MXT_DIR_NONE, cdir = MXT_DIR_FWD;
	int err = MXT_ERR_NONE;

	io_init();

	state = io_get_sensors();

	while(1)
	{
		int new_state = io_get_sensors();
		int reset = io_get_reset();
		int error_reset = io_get_error_reset();

		if(err)
		{
			if(error_reset)
			{
				io_set_error(err = MXT_ERR_NONE);
			}

			continue;
		}

		if(reset)
		{
			count = 0;
			fwd = MXT_DIR_NONE;
		}
		else if(new_state != state)
		{
			int step = 0;

			if(new_state == (state + 1) % MXT_STATES)
			{
				fwd = MXT_DIR_FWD;
				step = 1;
			}
			else if((new_state + 1) % MXT_STATES == state)
			{
				fwd = MXT_DIR_BCK;
				step = 1;
			}
			else
			{
				fwd = MXT_DIR_NONE;
				io_set_error(err = MXT_ERR_STEP);
			}

			if(step)
			{
				if(cdir == MXT_DIR_NONE || !count) cdir = fwd;

				count = (count + (cdir == fwd ? step : -step)) % MXT_STEPS;
			}
		}

		state = new_state;

		io_set_counter(count / MXT_STEPS);
		io_set_fwd_led(fwd);
		io_set_rotation(MXT_C_TO_R(count) * (cdir == MXT_DIR_BCK ? -1 : 1));
	}

	return EXIT_SUCCESS;
}

