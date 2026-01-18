#ifndef INPUT_H
#define INPUT_H

#include "../helpers/helpers.h"

/* input_utls.c */
int check_phone_numb(char *phone);
int is_alpha(int c);
int is_digit(int c);
int check_country(char *country);


/* safe_input_utils.c */
int safe_string_input(char *buffer, size_t size);
int safe_int_input(int *value);
int safe_float_input(float *value);

char confirm_char();

#endif