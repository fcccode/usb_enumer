#pragma once
#include "tinyxml\tinyxml.h"



#define ALLOC(dwBytes) GlobalAlloc(GPTR,(dwBytes))
#define FREE(hMem)     GlobalFree((hMem))

#define MAX_DRIVER_KEY_NAME 256
#define MAX_DEVICE_PROP 200
#define NUM_STRING_DESC_TO_GET 32

typedef struct
{
    CString sDeviceInstanceId;
    CString sDeviceDesc;
    CString sHardwareId;
    CString sService;
    CString sDeviceClass;
} UsbDevicePnpStrings, *PUsbDevicePnpStrings;

typedef struct _STRING_DESCRIPTOR_NODE
{
    struct _STRING_DESCRIPTOR_NODE *Next;
    UCHAR                           DescriptorIndex;
    USHORT                          LanguageID;
    USB_STRING_DESCRIPTOR           StringDescriptor[1];
} StringDescriptorNode, *PStringDescriptorNode;

typedef enum _TREEICON
{
    ComputerIcon,
    HubIcon,
    NoDeviceIcon,
    GoodDeviceIcon,
    BadDeviceIcon,
    GoodSsDeviceIcon,
    NoSsDeviceIcon
} TREEICON;

class CUsbEnumer
{
public:
    void EnumAllUsbDevices()
    {
        _EnumHostControllers();
    }

    BOOL IsAdbDevice(const CString& sFatherHubName, int nPortNum);

private:
    /**
     * ö�ٷ�Ϊ4��:
     * ����ö�����е�usb host controller,��Ӧ_EnumHostControllers()����
     * ��ĳ�������usb host controller����ö��,��Ӧ_EnumHostController()����.
     * ö��usb host controllerʱ,��ʵ�Ƕ���hub�ڽ���ö��,��Ӧ_EnumHub()����
     * ö��hub�ϵĸ���port,��Ӧ����_EnumHubPorts()
     *
     * һ̨��������ж��usb host controller.
     * һ��usb host controller����һ��root hub.
     * һ��root hub����չ�����ɸ�port��.
     * һ��port����,���ܲ���hub,Ҳ���ܲ��˾����豸(���ֻ�),Ҳ����ʲô��û��.
     */
    VOID _EnumHostControllers();
    VOID _EnumHostController(HANDLE hHCDev, HDEVINFO deviceInfo, PSP_DEVINFO_DATA deviceInfoData);
    VOID _EnumHub(const CString& sHubName);
    VOID _EnumHubPorts(HANDLE hHubDevice, ULONG NumPorts);

private:
    PUSB_DESCRIPTOR_REQUEST _GetConfigDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex);
    PStringDescriptorNode   _GetStringDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, USHORT LanguageID);

    PUSB_COMMON_DESCRIPTOR  _GetNextDescriptor(
        _In_reads_bytes_(TotalLength) PUSB_COMMON_DESCRIPTOR FirstDescriptor,
        _In_ ULONG TotalLength,
        _In_ PUSB_COMMON_DESCRIPTOR StartDescriptor,
        _In_ long DescriptorType
        );
    PUSB_COMMON_DESCRIPTOR  _NextDescriptor(_In_ PUSB_COMMON_DESCRIPTOR Descriptor);

    void _MyReadUsbDescriptorRequest(PUSB_DESCRIPTOR_REQUEST pRequest, BOOL& bFindInterface0xff42);
    void _ParsepUsbDescriptorRequest( PUSB_DESCRIPTOR_REQUEST pRequest);

private:
    // ��:"\\?\pci#ven_8086&dev_1e26&subsys_05771028&rev_04#3&11583659&1&e8#{3abf6f2d-71c4-462a-8a92-1e6861e6af27}"
    CString _GetDevPath( HDEVINFO hDevInfo, SP_DEVICE_INTERFACE_DATA stDeviceInterfaceData );
    CString _GetRootHubName(HANDLE hHostController);
    CString _GetExternalHubName(HANDLE Hub, ULONG ConnectionIndex);
    CString _GetHCDDriverKeyName(HANDLE hHCDev);
    CString _GetDriverKeyName(HANDLE Hub, ULONG ConnectionIndex); // ��:"{36fc9e60-c465-11cf-8056-444553540000}\0006"
    CString _GetDeviceProperty(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, DWORD Property);
    BOOL    _DriverNameToDeviceProperties(const CString& sDrvKeyName, UsbDevicePnpStrings& stPnpStrings);
    void    _GetConnInfo(
        ULONG ulConnectionIndex,
        HANDLE hHubDevice,
        PUSB_NODE_CONNECTION_INFORMATION_EX*    ppConnInfoEx,
        PUSB_NODE_CONNECTION_INFORMATION_EX_V2* ppConnInfoExV2
        );
    HDEVINFO _DriverNameToDeviceInst(const CString& sDrvKeyName, SP_DEVINFO_DATA& stDevInfoData);
};
