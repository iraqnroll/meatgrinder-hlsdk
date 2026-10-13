/*
mg_client.cpp - Meatgrinder client-side setup
*/

#include <stdarg.h>
#include <stdio.h>

#include "hud.h"
#include "cl_util.h"
#include "mg_client.h"
#include "mg_data.h"

// ---------------------------------------------------------------------------
// Data files
// TODO: once server -> client data sync lands, the client stops reading files
// and builds its copy from the server's messages instead.
// ---------------------------------------------------------------------------

static unsigned char *MG_ClientLoadFile( const char *path, int *length )
{
	return gEngfuncs.COM_LoadFile( path, 5, length ); // 5 = malloc'd, freed by COM_FreeFile
}

static void MG_ClientFreeFile( void *buffer )
{
	gEngfuncs.COM_FreeFile( buffer );
}

static void MG_ClientLog( const char *fmt, ... )
{
	char text[1024];
	va_list args;

	va_start( args, fmt );
	vsnprintf( text, sizeof( text ), fmt, args );
	va_end( args );

	gEngfuncs.Con_Printf( "%s", text );
}

// Reloads the client's copy (used by weapon prediction), then the server's.
// On a listen server sv_reload_data runs locally; on a remote server it's ignored.
static void MG_ClientReloadData( void )
{
	MG_DataReload();
	gEngfuncs.pfnClientCmd( "sv_reload_data\n" );
}

// ---------------------------------------------------------------------------

void MG_ClientInit( void )
{
	MG_DataInit( MG_ClientLoadFile, MG_ClientFreeFile, MG_ClientLog );
	MG_DataReload();
	gEngfuncs.pfnAddCommand( "reload_data", MG_ClientReloadData );
}

void MG_ClientShutdown( void )
{
	MG_DataShutdown();
}
