#include "MessageHandler.h"

bool MessageHandler::isMessageValid(const String &msg, const String &machineId)
{
    if (msg.length() < 3)
    {
        return false;
    }

    return msg.startsWith(machineId);
}

String MessageHandler::getValue(const String &data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
