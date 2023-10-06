#include "revert_string.h"

void RevertString(char *str)
{
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    // Переворачиваем строку
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

