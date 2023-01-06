#pragma once

class i_net_channel {
private:
	uint8_t u0[0x17];
public:
	bool should_delete;
	int out_sequence_nr;
	int in_sequence_nr;
	int out_sequence_nr_ack;
	int out_reliable_state;
	int in_reliable_state;
	int choked_packets;
};


class INetMessage {
public:
    virtual    ~INetMessage () { };
    // Use these to setup who can hear whose voice.
    // Pass in client indices (which are their ent indices - 1).
    virtual void SetNetChannel ( i_net_channel* netchan ) = 0; // netchannel this message is from/for
    virtual void SetReliable ( bool state ) = 0;    // set to true if it's a reliable message
    virtual bool Process ( void ) = 0; // calles the recently set handler to process this message
    virtual void __unknown1 ( void ) const = 0;
    virtual void __unknown2 ( void ) const = 0;
    virtual bool IsReliable ( void ) const = 0;  // true, if message needs reliable handling
    virtual int GetType ( void ) const = 0; // returns module specific header tag eg svc_serverinfo
    virtual int GetGroup ( void ) const = 0;    // returns net message group of this message
    virtual const char* GetName ( void ) const = 0; // returns a human readable string about message content
    virtual i_net_channel* GetNetChannel ( void ) const = 0;
    virtual const char* ToString ( void ) const = 0;
    virtual void __unknown5 ( void ) const = 0;
};