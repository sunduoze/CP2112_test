 /*++

	VER_LEGALCOPYRIGHT_STR

Module Name:

    silabs_version.h

Abstract:

    A top-level, authoritative, definition of some common across-all-repos/products items
	
	Versioning based on http://semver.org/ three-field version number, but extended to Microsoft's
	four-field version number. Semver's MAJOR.MINOR.PATCH extended to MAJOR.MINOR.PATCH.BUILD. Such that
	1.MAJOR version when you make incompatible API changes,
	2.MINOR version when you add functionality in a backwards-compatible manner,
	3.PATCH version when you make backwards-compatible bug fixes, and
	4.BUILD is Jenkins's BUILD_NUMBER.
	
	Notice too, that this file contains the "Product"-level version. Each build artifact within the
	"product" will have it's own, independent, "File"-level version number.

	Beyond a Version number, this file contains attributes that go into a Windows .RC VERSION resource (such as
	company name, copyright verbiage, etc.)

Environment:

    User or kernel mode

--*/
//
#ifndef	HOST_COMMON_INCLUDE_SILABS_VERSION_H_INCLUDED_I1WFULXDGP
#define	HOST_COMMON_INCLUDE_SILABS_VERSION_H_INCLUDED_I1WFULXDGP

#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ )

#define VER_COMPANYNAME_LEGAL_STR	"Silicon Laboratories Inc."
#define VER_COMPANYNAME_SHORT_STR	"Silicon Labs"
#define VER_COMPANYNAME_ABBR_STR	"SiLabs"

#define VER_LEGALCOPYRIGHT_STR		"Copyright © 2018 " VER_COMPANYNAME_LEGAL_STR ", All Rights Reserved."

#define VER_PRODUCTNAME_STR			VER_COMPANYNAME_SHORT_STR " USBXpress Software"

// Stringize an int
#define XSTRINGIFY2(x) #x
#define STRINGIFY(x) XSTRINGIFY2(x)
#define STR(x)		 XSTRINGIFY2(x)	// obsolete, don't use STR() any more

// Jenkins will pass in the BUILD_NUMBER. If not, use 0.
// Some futzing necessary to detect BUILD_NUMBER= which makes BUILD_NUMBER defined but to have no value, which breaks.
#if	! defined(BUILD_NUMBER) || (7-BUILD_NUMBER-7 == 14)
#undef BUILD_NUMBER	// undefine it, lest it be defined as no value (otherwise the next line throws a warning about a redefine)
#define	BUILD_NUMBER	(0)
#endif


#define VER_PRODUCTVERSION_MAJOR          6
#define VER_PRODUCTVERSION_MINOR          7
#define VER_PRODUCTVERSION_PATCH          6
#define VER_PRODUCTVERSION_BUILD          BUILD_NUMBER

#define VER_PRODUCTVERSION          VER_PRODUCTVERSION_MAJOR,VER_PRODUCTVERSION_MINOR,VER_PRODUCTVERSION_PATCH,VER_PRODUCTVERSION_BUILD
#define VER_PRODUCTVERSION_STR      STRINGIFY(VER_PRODUCTVERSION_MAJOR) "." STRINGIFY(VER_PRODUCTVERSION_MINOR) "." STRINGIFY(VER_PRODUCTVERSION_PATCH) "." STRINGIFY(VER_PRODUCTVERSION_BUILD)

#define RELEASE_TAG					VER_PRODUCTVERSION_STR

// Pointedly UNdefine Internal- and Original-filename so that it MUST be defined in
// respective files' .rc files.
// Original File Name - "Original name of the file, not including a path" -- MSDN
// Internal File Name - "original filename, without extension" -- MSDN
// For example, Original file name is "fubar.exe", giving an Internal file name of "fubar".
#undef VER_ORIGINALFILENAME_STR
#undef VER_INTERNALFILENAME_STR	

// Pointedly UNdefine the FileVersion so that it MUST be defined in
// respective files' .rc files.
#undef	VER_FILEVERSION
#undef	VER_FILEVERSION_STR

#endif // !defined(HOST_COMMON_INCLUDE_SILABS_VERSION_H_INCLUDED_I1WFULXDGP)

