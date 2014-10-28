#include "Transport.h"
#include "MailMessage.h"

#include <iostream>

using namespace std;

Transport::Transport(void)
{
}

Transport::~Transport(void)
{
}

/* static */ void Transport::Send(const MailMessage& message)
{
    (void)message;
    cout << "sending email!" << endl;
	throw 1;
}
