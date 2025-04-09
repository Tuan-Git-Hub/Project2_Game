#include "DataSettingMusic.h"
#include "Utilities.h"

using namespace ax;

DataSettingMusic& DataSettingMusic::getInstance()
{
    static DataSettingMusic instance;
    return instance;
}