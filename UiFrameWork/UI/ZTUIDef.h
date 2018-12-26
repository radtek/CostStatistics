#pragma once


#ifdef ZTUIDLL_EXPORTS
	#define ZTUIDLL_API __declspec(dllexport)
#else
	#ifdef USE_ZTUILIB
		#define ZTUIDLL_API
	#else
		#ifdef USE_ZTUISRC
			#define ZTUIDLL_API
		#else
			#define ZTUIDLL_API __declspec(dllimport)
		#endif
	#endif
#endif
