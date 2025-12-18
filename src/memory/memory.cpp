#include "memory.h"

NtDll::NtDll() { base_address = (uintptr_t)GetModuleHandleA("ntdll.dll"); }

Memory::Memory(std::string _process_name, std::string _window_name) {
    process_id = 0;
    window_handle = nullptr;
    process_name = _process_name;
    window_name = _window_name;

    if (!process_name.empty()) {
        attach_process(process_name);
        window_handle = get_window_handle();
    } else if (!window_name.empty()) {
        attach_window(window_name);
    } else {
        assert(false && "Either process_name or window_name must be provided.");
    }
}

bool Memory::attach_process(std::string process_name) {
    process_id = find_process_id(process_name);
    process_handle = nt_open_process(process_id);
    return process_handle != 0;
}

bool Memory::attach_window(std::string window_name) {
    window_handle = FindWindowA(0, window_name.c_str());
    GetWindowThreadProcessId(window_handle, &process_id);
    process_handle = nt_open_process(process_id);
    return process_handle != 0;
}

HWND Memory::get_window_handle() {
    if (window_handle && IsWindow(window_handle))
        return window_handle;

    HWND hwnd = FindWindowA(nullptr, "Roblox");
    if (hwnd) {
        DWORD window_pid = 0;
        GetWindowThreadProcessId(hwnd, &window_pid);
        if (window_pid == process_id && IsWindowVisible(hwnd)) {
            window_handle = hwnd;
            return window_handle;
        }
    }

    struct EnumData {
        DWORD pid;
        HWND found;
    };

    EnumData data{process_id, nullptr};
    EnumWindows(
        [](HWND hwnd, LPARAM lParam) -> BOOL {
            EnumData* pData = reinterpret_cast<EnumData*>(lParam);
            DWORD window_pid = 0;
            GetWindowThreadProcessId(hwnd, &window_pid);
            if (window_pid == pData->pid && IsWindowVisible(hwnd)) {
                pData->found = hwnd;
                return FALSE;
            }
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&data));

    window_handle = data.found;
    return window_handle;
}

uintptr_t Memory::base_address() {
    static auto base = uintptr_t(get_module_by_name(process_id, process_name).modBaseAddr);
    return base;
}

std::pair<int, int> Memory::get_window_dimensions() {
    if (!window_handle)
        return {0, 0};

    RECT rect;
    if (GetClientRect(window_handle, &rect)) {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        return {width, height};
    }
    return {0, 0};
}

DWORD Memory::find_process_id(std::string process_name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32W pe = {sizeof(PROCESSENTRY32W)};
    std::wstring wide_process_name = std::wstring(process_name.begin(), process_name.end());

    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, wide_process_name.c_str()) == 0) {
                CloseHandle(snapshot);
                return pe.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return 0;
}

HANDLE Memory::nt_open_process(DWORD pid) {
    using tNtOpenProcess = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
    static tNtOpenProcess fn = ntdll.get_exported_function<tNtOpenProcess>("NtOpenProcess");

    OBJECT_ATTRIBUTES obj_attrs;
    ZeroMemory(&obj_attrs, sizeof(obj_attrs));
    obj_attrs.Length = sizeof(obj_attrs);

    CLIENT_ID client_id;
    client_id.UniqueProcess = (HANDLE)(uintptr_t)pid;
    client_id.UniqueThread = 0;

    HANDLE h_proc = 0;
    NTSTATUS status = fn(&h_proc, PROCESS_ALL_ACCESS, &obj_attrs, &client_id);
    return NT_SUCCESS(status) ? h_proc : 0;
}

MODULEENTRY32W Memory::get_module_by_name(DWORD pid, std::string name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    MODULEENTRY32W mod = {sizeof(MODULEENTRY32W)};
    std::wstring wide_name = std::wstring(name.begin(), name.end());

    if (Module32FirstW(snapshot, &mod)) {
        do {
            if (_wcsicmp(mod.szModule, wide_name.c_str()) == 0) {
                CloseHandle(snapshot);
                return mod;
            }
        } while (Module32NextW(snapshot, &mod));
    }

    CloseHandle(snapshot);
    return {};
}

std::string Memory::get_executable_path() {
    MODULEENTRY32W mod = get_module_by_name(process_id, process_name);
    if (mod.szExePath[0] == 0)
        return "";

    std::wstring wide_path(mod.szExePath);
    return std::string(wide_path.begin(), wide_path.end());
}

std::vector<uint8_t> Memory::read_bytes(uintptr_t address, size_t size) {
    using tNtReadVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    static tNtReadVirtualMemory fn =
        ntdll.get_exported_function<tNtReadVirtualMemory>("NtReadVirtualMemory");

    std::vector<uint8_t> buffer(size);
    SIZE_T bytes_read = 0;
    NTSTATUS status =
        fn(process_handle, reinterpret_cast<PVOID>(address), buffer.data(), size, &bytes_read);

    if (!NT_SUCCESS(status) || bytes_read != size)
        return {};
    return buffer;
}

bool Memory::read_raw(uintptr_t address, void* buffer, size_t size) {
    using tNtReadVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    static tNtReadVirtualMemory fn =
        ntdll.get_exported_function<tNtReadVirtualMemory>("NtReadVirtualMemory");

    SIZE_T bytes_read = 0;
    NTSTATUS status =
        fn(process_handle, reinterpret_cast<PVOID>(address), buffer, size, &bytes_read);
    return NT_SUCCESS(status) && bytes_read == size;
}

std::string Memory::read_string(uintptr_t address, size_t max_length) {
    if (!address)
        return "";

    auto bytes = read_bytes(address, max_length);
    if (bytes.empty())
        return "";

    auto null_pos = std::find(bytes.begin(), bytes.end(), '\0');
    std::string result(bytes.begin(), null_pos);

    int valid_chars = 0;
    for (char c : result) {
        if ((c >= 32 && c <= 126) || c == 0)
            valid_chars++;
    }

    if (result.empty() || valid_chars < result.length() * 0.8)
        return "";
    return result;
}

std::string Memory::read_roblox_string(uintptr_t address) {
    if (!address)
        return "";

    std::int32_t length = read<std::int32_t>(address + 0x18);
    if (length <= 0 || length > 1024)
        return "";

    std::uint64_t data_ptr = (length >= 16) ? read<std::uint64_t>(address + 0x0) : address;
    if (!data_ptr)
        return "";

    return read_string(data_ptr, length);
}

std::optional<std::pair<uintptr_t, size_t>> Memory::get_section_range(const char* section_name) {
    uintptr_t base = base_address();

    IMAGE_DOS_HEADER dos{};
    if (!read_raw(base, &dos, sizeof(dos)) || dos.e_magic != IMAGE_DOS_SIGNATURE)
        return std::nullopt;

    IMAGE_NT_HEADERS64 nt{};
    if (!read_raw(base + dos.e_lfanew, &nt, sizeof(nt)) || nt.Signature != IMAGE_NT_SIGNATURE)
        return std::nullopt;

    WORD number_of_sections = nt.FileHeader.NumberOfSections;
    WORD opt_header_size = nt.FileHeader.SizeOfOptionalHeader;

    uintptr_t section_header_addr =
        base + dos.e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + opt_header_size;

    for (WORD i = 0; i < number_of_sections; ++i) {
        IMAGE_SECTION_HEADER sec{};
        if (!read_raw(section_header_addr + i * sizeof(IMAGE_SECTION_HEADER), &sec,
                      sizeof(IMAGE_SECTION_HEADER)))
            return std::nullopt;

        char name[9]{};
        std::memcpy(name, sec.Name, 8);

        if (std::strncmp(name, section_name, 8) == 0) {
            uintptr_t start = base + sec.VirtualAddress;
            size_t size = sec.Misc.VirtualSize;
            return std::make_pair(start, size);
        }
    }

    return std::nullopt;
}

std::optional<size_t> Memory::find_verified_offset_float(const std::vector<uintptr_t>& addresses,
                                                         const std::vector<float>& expected_values,
                                                         size_t max_offset, size_t alignment,
                                                         float tolerance) {
    if (addresses.size() != expected_values.size() || addresses.empty())
        return std::nullopt;

    std::vector<size_t> candidates;
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        float value = read<float>(addresses[0] + offset);
        if (std::abs(value - expected_values[0]) < tolerance)
            candidates.push_back(offset);
    }

    for (size_t offset : candidates) {
        bool all_match = true;
        for (size_t i = 1; i < addresses.size(); i++) {
            float value = read<float>(addresses[i] + offset);
            if (std::abs(value - expected_values[i]) >= tolerance) {
                all_match = false;
                break;
            }
        }
        if (all_match)
            return offset;
    }
    return std::nullopt;
}

std::optional<RobloxStringInfo> Memory::scan_roblox_string(uintptr_t instance_address,
                                                           const std::string& expected_name,
                                                           size_t max_pointer_offset,
                                                           size_t alignment) {
    constexpr size_t length_offset = 0x18;
    for (size_t ptr_offset = 0; ptr_offset < max_pointer_offset; ptr_offset += alignment) {
        uintptr_t string_obj_ptr = read<uintptr_t>(instance_address + ptr_offset);
        if (!string_obj_ptr || string_obj_ptr < 0x10000)
            continue;

        std::string read_name = read_roblox_string(string_obj_ptr);
        if (read_name == expected_name) {
            RobloxStringInfo info;
            info.pointer_offset = ptr_offset;
            info.length_offset = length_offset;
            return info;
        }
    }
    return std::nullopt;
}

std::optional<size_t> Memory::find_roblox_string_direct(uintptr_t base_address,
                                                        const std::string& expected_name,
                                                        size_t max_offset, size_t alignment) {
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        uintptr_t string_address = base_address + offset;
        std::string read_name = read_roblox_string(string_address);
        if (read_name == expected_name)
            return offset;
    }
    return std::nullopt;
}

std::optional<size_t> Memory::find_string_direct(uintptr_t base_address,
                                                 const std::string& expected_string,
                                                 size_t max_offset, size_t alignment,
                                                 size_t max_string_length) {
    for (size_t offset = 0; offset < max_offset; offset += alignment) {
        uintptr_t string_address = base_address + offset;
        std::string read_str = read_string(string_address, max_string_length);
        if (read_str == expected_string)
            return offset;
    }
    return std::nullopt;
}