#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const char *month_names[] = {"January", "February", "March", "April", "May", "June",
                                    "July", "August", "September", "October", "November", "December"};
static const char *day_names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday",};

int is_leap_year(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


void timestamp_to_datetime(uint64 timestamp, int *year, int *month, int *day, int *day_of_week,
                           int *hour, int *minute, int *second, int *fractional_second) {

  int is_before_epoch = 0;


  *fractional_second = timestamp % 1000000000;
  long seconds = (long) (timestamp);
  seconds /= 1000000000;

  *second = seconds % 60;
  seconds /= 60;
  *minute = seconds % 60;
  seconds /= 60;
  *hour = seconds % 24;
  seconds /= 24;

  *year = 1970;
  *day_of_week = 3;


    while (1) {
      int days_in_year = is_leap_year(*year) ? 366 : 365;
      if (seconds < days_in_year)
        break;
      seconds -= days_in_year;
      *day_of_week = (*day_of_week + days_in_year) % 7;
      (*year)++;
    }
  }
  *month = 0;
  while (*month < 12) {
    int days = days_in_month[*month];
    if (*month == 1 && is_leap_year(*year))
      days++;
    if (seconds < days)
      break;
    seconds -= days;
    *day_of_week = (*day_of_week + days) % 7;
    (*month)++;
  

  *day = seconds + 1;
  *day_of_week = (*day_of_week + seconds) % 7;

}

int main(int argc, char *argv[]) {
  uint64 time;

  rtc(&time);


  fprintf(0, "Raw timestamp: %lx %lx\n", time >> 32, time);

  int year, month, day, hour, minute, second, weekday, fractional_second;
  timestamp_to_datetime(time, &year, &month, &day, &weekday, &hour, &minute, &second, &fractional_second);

  fprintf(0, "Date: %s, %s %d, %d at %d:%d:%d.%d\n",
          day_names[weekday],
          month_names[month], day, year,
          hour, minute, second, fractional_second);

  return 0;
}