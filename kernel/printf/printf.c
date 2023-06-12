#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "../sys/print.h"

void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int int2str(char* buffer, int value) {
    int i = 0;
    int is_negative = 0;

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    do {
        buffer[i++] = value % 10 + '0';
        value /= 10;
    } while (value != 0);

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    reverse(buffer, i);

    return i;
}

void uintptr2str(char* buffer, uintptr_t value) {
    int i = 0;

    do {
        buffer[i++] = value % 16 < 10 ? (value % 16) + '0' : (value % 16) - 10 + 'A';
        value /= 16;
    } while (value != 0);

    buffer[i] = '\0';

    reverse(buffer, i);
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (unsigned int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;

            switch (format[i]) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[12];

                    int length = int2str(buffer, value);

                    print(buffer);

                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    char buffer[20];

                    uintptr2str(buffer, (uintptr_t)ptr);

                    print(buffer);

                    break;
                }
                default:
                    break;
            }
        } else {
            char character[2] = { format[i], '\0' };
            print(character);
        }
    }

    va_end(args);
}
