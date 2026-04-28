#pragma once
#include "public/mysql_mm.h"

class SQLInterface : public IMySQLInterface
{
    IMySQLClient *GetMySQLClient();
};
