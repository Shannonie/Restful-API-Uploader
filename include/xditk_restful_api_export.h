#ifndef XDITK_RESTFUL_API_EXPORT_H
#define XDITK_RESTFUL_API_EXPORT_H

#if defined RESTFUL_API_EXPORT_EXPORT_BUILT_AS_STATIC
#    define xditk_restful_api_export
#else
#   ifdef RESTFUL_API_EXPORT
/* We are building this library */
#       define xditk_restful_api_export __declspec(dllexport)
#   else
/* We are using this library */
#       define xditk_restful_api_export __declspec(dllimport)
#   endif
#endif

#endif // XDITK_RESTFUL_API_EXPORT_H