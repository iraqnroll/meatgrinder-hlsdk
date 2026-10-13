/*
mg_client.h - Meatgrinder client-side setup

All Meatgrinder client systems are initialized from MG_ClientInit (called
from HUD_Init) and torn down in MG_ClientShutdown (called from HUD_Shutdown).
*/

#pragma once
#if !defined( MG_CLIENT_H )
#define MG_CLIENT_H

void MG_ClientInit( void );
void MG_ClientShutdown( void );

#endif // MG_CLIENT_H
