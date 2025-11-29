#include "../atm_sys.h"

// Safe string input
int safe_string_input(char *buffer, size_t size)
{
	if (!buffer)
		return 0;

	if (size == 0)
		size = MAX_STR_LEN;

	if (fgets(buffer, size, stdin) == NULL)
	{
		buffer[0] = '\0';
		return 0;
	}

	buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
	return 1;
}

// Safe integer input
int safe_int_input(int *value)
{
	char temp[50];
	if (!safe_string_input(temp, sizeof(temp)))
		return 0;

	char *endptr;
	long val = strtol(temp, &endptr, 10);

	if (*endptr != '\0')
		return 0;

	*value = (int)val;
	return 1;
}

// Safe float input
int safe_float_input(float *value)
{
	char temp[50];
	if (!safe_string_input(temp, sizeof(temp)))
		return 0;

	char *endptr;
	float val = strtof(temp, &endptr);

	// Check if *anything* was parsed
	if (endptr == temp)
		return 0;

	// Check for leftover garbage characters
	if (*endptr != '\0')
		return 0;

	*value = val;
	return 1;
}

static int prompt_until_valid(Record *info, int (*validator)(Record *),
							  const char *err_msg)
{
	int attempts = 0;
	while (attempts++ < MAX_ATTEMPTS)
	{
		system("clear");
		printf("\t\t\t===== New record =====\n");

		if (validator(info))
		{
			return 1; /* success */
		}

		/* validation failed: show message then retry */
		system("clear");
		printf("%s\n", err_msg ? err_msg : "Invalid input!\n");
		sleep(2);
	}
	/* reached max attempts */
	return 0;
}