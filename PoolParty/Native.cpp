#include "Native.hpp"

// TODO: RtlNtStatusToDosError may help for NativeWindowsException

void w_ZwAssociateWaitCompletionPacket(
    HANDLE WaitCopmletionPacketHandle,
    HANDLE IoCompletionHandle,
    HANDLE TargetObjectHandle,
    PVOID KeyContext,
    PVOID ApcContext,
    NTSTATUS IoStatus,
    ULONG_PTR IoStatusInformation,
    PBOOLEAN AlreadySignaled
) 
{
    auto Ntstatus = ZwAssociateWaitCompletionPacket(
        WaitCopmletionPacketHandle,
        IoCompletionHandle,
        TargetObjectHandle,
        KeyContext, 
        ApcContext,
        IoStatus,
        IoStatusInformation,
        AlreadySignaled);
    if (!NT_SUCCESS(Ntstatus)) 
    {
        throw WindowsException("ZwAssociateWaitCompletionPacket"); // TODO: Convert to NativeWindowsException which will display status string representation
    }
}

void w_ZwSetInformationFile(
    HANDLE hFile,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID FileInformation,
    ULONG Length,
    ULONG FileInformationClass
)
{
    auto Ntstatus = ZwSetInformationFile(hFile, IoStatusBlock, FileInformation, Length, FileInformationClass);
    if (!NT_SUCCESS(Ntstatus))
    {
        throw WindowsException("ZwSetInformationFile"); // TODO: Convert to NativeWindowsException which will display status string representation
    }
}

HANDLE w_NtAlpcCreatePort(POBJECT_ATTRIBUTES ObjectAttributes, PALPC_PORT_ATTRIBUTES PortAttributes) {
    HANDLE hAlpc;
    auto Ntstatus = NtAlpcCreatePort(&hAlpc, ObjectAttributes, PortAttributes);
    if (!NT_SUCCESS(Ntstatus))
    {
        throw WindowsException("NtAlpcCreatePort"); // TODO: Convert to NativeWindowsException which will display status string representation
    }

    return hAlpc;
}

void w_NtAlpcSetInformation(HANDLE hAlpc, ULONG PortInformationClass, PVOID PortInformation, ULONG Length) 
{
    auto Ntstatus = NtAlpcSetInformation(hAlpc, PortInformationClass, PortInformation, Length);
    if (!NT_SUCCESS(Ntstatus))
    {
        throw WindowsException("NtAlpcSetInformation"); // TODO: Convert to NativeWindowsException which will display status string representation
    }
}


HANDLE w_NtAlpcConnectPort(
    PUNICODE_STRING PortName,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PALPC_PORT_ATTRIBUTES PortAttributes,
    DWORD ConnectionFlags,
    PSID RequiredServerSid,
    PPORT_MESSAGE ConnectionMessage,
    PSIZE_T ConnectMessageSize,
    PALPC_MESSAGE_ATTRIBUTES OutMessageAttributes,
    PALPC_MESSAGE_ATTRIBUTES InMessageAttributes,
    PLARGE_INTEGER Timeout
) 
{
    HANDLE hAlpc;
    auto Ntstatus = NtAlpcConnectPort(
        &hAlpc,
        PortName, 
        ObjectAttributes,
        PortAttributes,
        ConnectionFlags, 
        RequiredServerSid,
        ConnectionMessage,
        ConnectMessageSize,
        OutMessageAttributes,
        InMessageAttributes, 
        Timeout);
    if (!NT_SUCCESS(Ntstatus)) 
    {
        throw WindowsException("NtAlpcConnectPort"); // TODO: Convert to NativeWindowsException which will display status string representation
    }

    return hAlpc;
}

std::vector<BYTE> w_NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass) 
{
    ULONG SystemInformationLength = 0;
    auto Ntstatus = STATUS_INFO_LENGTH_MISMATCH;
    std::vector<BYTE> SystemInformation;

    do 
    {
        SystemInformation.resize(SystemInformationLength);
        Ntstatus = NtQuerySystemInformation(SystemInformationClass, SystemInformation.data(), SystemInformationLength, &SystemInformationLength);
    } while (Ntstatus == STATUS_INFO_LENGTH_MISMATCH);

    if (!NT_SUCCESS(Ntstatus)) 
    {
        throw WindowsException("NtQuerySystemInformation");
    }

    return SystemInformation;
}

std::vector<BYTE> w_NtQueryObject(HANDLE hObject, OBJECT_INFORMATION_CLASS ObjectInformationClass)
{
    ULONG ObjectInformationLength = 0;
    auto Ntstatus = STATUS_INFO_LENGTH_MISMATCH;
    std::vector<BYTE> ObjectInformation;

    do {
        ObjectInformation.resize(ObjectInformationLength);
        Ntstatus = NtQueryObject(hObject, ObjectInformationClass, ObjectInformation.data(), ObjectInformationLength, &ObjectInformationLength);
    } while (Ntstatus == STATUS_INFO_LENGTH_MISMATCH);

    if (!NT_SUCCESS(Ntstatus)) {
        throw WindowsException("NtQueryObject");
    }

    return ObjectInformation;
}