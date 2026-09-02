class SparkZCoreLogger
{
    static void Fatal(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_FATAL);
    }

    static void Error(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_ERROR);
    }

    static void Warn(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_WARN);
    }

    static void Info(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_INFO);
    }

    static void Debug(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_DEBUG);
    }

    static void Verbose(string message)
    {
        Log(message, SparkZCoreLogLevel.SPARKZ_LOG_VERBOSE);
    }

    static void Log(string message, int level)
    {
        Print(SparkZCoreConstants.LOG_PREFIX + " " + LevelToString(level) + ": " + message);
    }

    static string LevelToString(int level)
    {
        switch (level)
        {
            case SparkZCoreLogLevel.SPARKZ_LOG_FATAL:
                return "FATAL";

            case SparkZCoreLogLevel.SPARKZ_LOG_ERROR:
                return "ERROR";

            case SparkZCoreLogLevel.SPARKZ_LOG_WARN:
                return "WARN";

            case SparkZCoreLogLevel.SPARKZ_LOG_DEBUG:
                return "DEBUG";

            case SparkZCoreLogLevel.SPARKZ_LOG_VERBOSE:
                return "VERBOSE";
        }

        return "INFO";
    }
}
