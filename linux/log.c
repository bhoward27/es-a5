#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "log.h"
#include "int_typedefs.h"

static uint8 gLogLevel = LOG_LEVEL_ERROR;

static bool isValidLogLevel(uint8 logLevel);

// You probably want to call LOG instead of this directly.
void logMsg(LogLevel logLevel, char* file, int line, const char* function, const char* format, ...)
{
    assert(isValidLogLevel(gLogLevel));
    if (logLevel <= gLogLevel) {
        assert(format != NULL);
        char logLevelTag[MAX_LOG_LEVEL_TAG_LEN];
        switch (logLevel) {
            case LOG_LEVEL_ERROR:
                strncpy(logLevelTag, "ERROR", MAX_LOG_LEVEL_TAG_LEN);
                break;
            case LOG_LEVEL_WARN:
                strncpy(logLevelTag, "WARN", MAX_LOG_LEVEL_TAG_LEN);
                break;
            case LOG_LEVEL_NOTICE:
                strncpy(logLevelTag, "NOTICE", MAX_LOG_LEVEL_TAG_LEN);
                break;
            case LOG_LEVEL_INFO:
                strncpy(logLevelTag, "INFO", MAX_LOG_LEVEL_TAG_LEN);
                break;
            case LOG_LEVEL_DEBUG:
                strncpy(logLevelTag, "DEBUG", MAX_LOG_LEVEL_TAG_LEN);
                break;
            default:
                assert(false);
                break;
        }
        char prefix[LOG_LEN];
        snprintf(prefix, LOG_LEN, "[%s:%d %s()] %s: ", file, line, function, logLevelTag);

        va_list pArgs;
        va_start(pArgs, format);
        char message[LOG_LEN * 2];
        vsnprintf(message, LOG_LEN * 2, format, pArgs);
        va_end(pArgs);

        printf("%s%s", prefix, message);
    }
}

int initLogLevel(void)
{
    char defaultMsg[LOG_LEN];
    snprintf(defaultMsg, LOG_LEN, "Defaulting to log level %d.\n", gLogLevel);

    FILE* pFile = fopen(LOG_LEVEL_PATH, "r");
    if (pFile == NULL) {
        FILE_OPEN_ERR(LOG_LEVEL_PATH, false);
        LOG(LOG_LEVEL_ERROR, defaultMsg);
        return ERR_OPEN;
    }

    char logLevelString[2];
    // Read a single character from LOG_LEVEL_PATH. This means numbers that take up more than 1 character, so negative
    // numbers or numbers greater than 9, will not be read properly. That's okay since our log levels don't go to that
    // range.
    size_t res = fread(logLevelString, sizeof(char), 1, pFile);
    if (res == 0) {
        FILE_READ_ERR(LOG_LEVEL_PATH, false);
        LOG(LOG_LEVEL_ERROR, defaultMsg);
        return ERR_READ;
    }

    if (fclose(pFile)) {
        FILE_CLOSE_ERR(LOG_LEVEL_PATH, false);
        LOG(LOG_LEVEL_ERROR, defaultMsg);
        return ERR_CLOSE;
    }

    logLevelString[1] = '\0';
    // atoi returns 0 if there's an error. That's okay.
    uint8 logLevel = (uint8) atoi(logLevelString);
    if (isValidLogLevel(logLevel)) {
        gLogLevel = logLevel;
    }
    else {
        LOG(LOG_LEVEL_ERROR, "Invalid log level '%s'.\n", logLevelString);
        LOG(LOG_LEVEL_ERROR, defaultMsg);
        return !OK;
    }

    return OK;
}

LogLevel getLogLevel(void)
{
    return gLogLevel;
}

static bool isValidLogLevel(uint8 logLevel)
{
    return (logLevel <= LOG_LEVEL_DEBUG && logLevel >= LOG_LEVEL_ERROR);
}