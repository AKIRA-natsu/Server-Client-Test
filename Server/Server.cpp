#include "Net/ServerSystem.h"

int main()
{
    auto system = AKIRA_Net::ServerSystem::GetInstance();
    system->Connect();
}