class SparkZBaseTime
{
    static int Now()
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;

        GetYearMonthDayUTC(year, month, day);
        GetHourMinuteSecondUTC(hour, minute, second);
        return ToUnixTimestamp(year, month, day, hour, minute, second);
    }

    protected static int ToUnixTimestamp(int year, int month, int day, int hour, int minute, int second)
    {
        int days = 0;
        for (int y = 1970; y < year; y++)
        {
            if (IsLeapYear(y))
                days = days + 366;
            else
                days = days + 365;
        }

        for (int m = 1; m < month; m++)
        {
            days = days + DaysInMonth(year, m);
        }

        days = days + day - 1;
        return days * 86400 + hour * 3600 + minute * 60 + second;
    }

    protected static bool IsLeapYear(int year)
    {
        if ((year % 400) == 0)
            return true;

        if ((year % 100) == 0)
            return false;

        return (year % 4) == 0;
    }

    protected static int DaysInMonth(int year, int month)
    {
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
            return 31;

        if (month == 4 || month == 6 || month == 9 || month == 11)
            return 30;

        if (IsLeapYear(year))
            return 29;

        return 28;
    }
}
