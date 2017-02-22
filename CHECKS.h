#ifndef CHECKS_H
#define CHECKS_H

#define CHECK(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }


#define CHECKINT(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }

#define CHECKDOUBLE(text, actual, expected)                         \
if ( ABS(actual,expected) > 0.01 ) {                                   \
  printf("%-30s: Failed | actual = %3f, expected = %3f\n",  \
         text,                                              \
         actual,                                            \
         expected);                                         \
} else {                                                    \
  printf("%-30s: Success\n", text);                     \
}

#define ABS(value1, value2) (value1 - value2 < 0 ) ? value2 - value1 : value1 - value2

void CHECK_TRENDS(char **,char **);

#endif
