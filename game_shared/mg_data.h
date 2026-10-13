/*
mg_data.h - Meatgrinder data files (JSON under <gamedir>/data/)

Compiled into both the client and the server. Each side passes its own
engine file functions to MG_DataInit, so the same lookups work in weapon
code that runs on both sides (client prediction + server).
*/

#pragma once
#if !defined( MG_DATA_H )
#define MG_DATA_H

typedef unsigned char *(*MG_LoadFileFn)( const char *path, int *length );
typedef void (*MG_FreeFileFn)( void *buffer );
typedef void (*MG_LogFn)( const char *fmt, ... );

void MG_DataInit( MG_LoadFileFn loadFile, MG_FreeFileFn freeFile, MG_LogFn log );
void MG_DataShutdown( void );

// (Re)load every data file. A file with a JSON error keeps its previous
// contents, so a typo while editing doesn't wipe the live values.
// Returns false if any file failed.
bool MG_DataReload( void );

// data/weapons.json: { "glock": { "primary_cycle": 0.3, ... }, ... }
float MG_WeaponFloat( const char *weapon, const char *key, float fallback );

#endif // MG_DATA_H
