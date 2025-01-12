#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <Arduino.h>

class MessageHandler
{
public:
    static bool isMessageValid(const String &msg, const String &machineId);
    static String getValue(const String &data, char separator, int index);
};

#endif
