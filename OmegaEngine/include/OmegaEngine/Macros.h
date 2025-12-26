#pragma once

#ifndef OMEGA_API
	#ifdef _WIN32
	#ifdef OMEGA_ENGINE_DLL 
		#define OMEGA_API __declspec(dllexport)
	#else
		#define OMEGA_API __declspec(dllimport)
	#endif
	#else
		#define OMEGA_API
	#endif
#endif