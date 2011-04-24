#pragma once

#include "qglobal.h"

#ifdef Q_WS_WIN

#include <windows.h>

#include <QWidget>

#include "CaptureSourceBase.hpp"

namespace lightpack
{
namespace capture
{
namespace dwmwrapper
{
    enum DWM_TMP_VALUES
    {
        DWM_TNP_RECTDESTINATION = 0x00000001,
        DWM_TNP_RECTSOURCE = 0x00000002,
        DWM_TNP_OPACITY = 0x00000004,
        DWM_TNP_VISIBLE = 0x00000008,
        DWM_TNP_SOURCECLIENTAREAONLY = 0x00000010
    };

    static const wchar_t * DWM_API_DLL = L"dwmapi.dll";
    static const char * DWM_REGISTER_THUMBNAIL = "DwmRegisterThumbnail";
    static const char * DWM_UPDATE_REGISTER_PROPERTIES = "DwmUpdateThumbnailProperties";
    static const char * DWM_UNREGISTER_THUMBNAIL = "DwmUnregisterThumbnail";

    typedef HRESULT DWMAPI;
    typedef HANDLE HTHUMBNAIL;
    typedef HTHUMBNAIL* PHTHUMBNAIL;

    typedef struct _DWM_THUMBNAIL_PROPERTIES
    {
        DWORD dwFlags;
        RECT rcDestination;
        RECT rcSource;
        BYTE opacity;
        BOOL fVisible;
        BOOL fSourceClientAreaOnly;
    } DWM_THUMBNAIL_PROPERTIES, *PDWM_THUMBNAIL_PROPERTIES;

    typedef DWMAPI (WINAPI * DwmRegisterThumbnail_t)(HWND hwndDestination, HWND hwndSource, /*out*/ PHTHUMBNAIL phThumbnailId);
    typedef DWMAPI (WINAPI * DwmUpdateThumbnailProperties_t)(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES *ptnProperties);
    typedef DWMAPI (WINAPI * DwmUnregisterThumbnail_t)(HTHUMBNAIL hThumbnailId);

    DWMAPI DwmRegisterThumbnail(HWND hwndDestination, HWND hwndSource, /*out*/ PHTHUMBNAIL phThumbnailId);
    DWMAPI DwmUpdateThumbnailProperties(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES *ptnProperties);
    DWMAPI DwmUnregisterThumbnail(HTHUMBNAIL hThumbnailId);
}
}
}

namespace lightpack
{
namespace capture
{
    using namespace dwmwrapper;

    class CaptureSourceWindowsDWM : CaptureSourceBase
    {
    private:
        HWND m_rectHWnd;

    public:
        CaptureSourceWindowsDWM();

    // CaptureSourceBase
    protected:
        virtual void fillData();
        virtual void fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer);
    };
}
}

#endif
