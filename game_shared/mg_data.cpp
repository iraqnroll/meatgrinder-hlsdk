/*
mg_data.cpp - Meatgrinder data files (JSON under <gamedir>/data/)
*/

#include <stddef.h>
#include "mg_data.h"
#include "../external/cjson/cJSON.h"

static MG_LoadFileFn s_loadFile;
static MG_FreeFileFn s_freeFile;
static MG_LogFn s_log;

static cJSON *s_weapons;

void MG_DataInit( MG_LoadFileFn loadFile, MG_FreeFileFn freeFile, MG_LogFn log )
{
	s_loadFile = loadFile;
	s_freeFile = freeFile;
	s_log = log;
}

void MG_DataShutdown( void )
{
	cJSON_Delete( s_weapons );
	s_weapons = NULL;
}

// Parses one file. On failure logs the line of the error and returns NULL.
static cJSON *MG_ParseFile( const char *path )
{
	int length = 0;
	unsigned char *buffer = s_loadFile ? s_loadFile( path, &length ) : NULL;

	if( !buffer )
	{
		if( s_log )
			s_log( "[mg_data] can't open %s\n", path );
		return NULL;
	}

	const char *text = (const char *)buffer;
	const char *end = NULL;
	cJSON *root = cJSON_ParseWithLengthOpts( text, length, &end, 0 );

	if( !root && s_log )
	{
		int line = 1;
		for( const char *p = text; end && p < end; p++ )
		{
			if( *p == '\n' )
				line++;
		}
		s_log( "[mg_data] %s: JSON error on line %d, keeping previous values\n", path, line );
	}

	s_freeFile( buffer );
	return root;
}

// Replaces *slot only if the file parsed.
static bool MG_ReloadFile( const char *path, cJSON **slot )
{
	cJSON *root = MG_ParseFile( path );

	if( !root )
		return false;

	cJSON_Delete( *slot );
	*slot = root;
	return true;
}

bool MG_DataReload( void )
{
	bool ok = true;

	ok &= MG_ReloadFile( "data/weapons.json", &s_weapons );

	if( ok && s_log )
		s_log( "[mg_data] data loaded\n" );

	return ok;
}

float MG_WeaponFloat( const char *weapon, const char *key, float fallback )
{
	const cJSON *entry = cJSON_GetObjectItemCaseSensitive( s_weapons, weapon );
	const cJSON *value = cJSON_GetObjectItemCaseSensitive( entry, key );

	return cJSON_IsNumber( value ) ? (float)value->valuedouble : fallback;
}
