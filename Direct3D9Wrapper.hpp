#ifndef _DIRECT3D9_WRAPPER_HPP_
#define _DIRECT3D9_WRAPPER_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <d3d9.h>

class Direct3D9Wrapper
{
    static constexpr auto pGameDevice{ 0xC97C28 };
    static constexpr auto pGameHWND{ 0xC17054 };
public:
    static IDirect3DDevice9* getDevice() {
        return *reinterpret_cast<IDirect3DDevice9**>(pGameDevice);
    }
    static HWND getHWND() {
        return **reinterpret_cast<HWND**>( pGameHWND );
    }
    static std::uintptr_t findDevice(std::uint32_t Len) {
        static std::uintptr_t base = [](std::size_t Len) {
            std::string path_to(MAX_PATH, '\0');
            if (auto size = GetSystemDirectoryA(path_to.data(), MAX_PATH)) {
                path_to.resize(size);
                path_to += "\\d3d9.dll";
                std::uintptr_t dwObjBase = reinterpret_cast<std::uintptr_t>(LoadLibraryA(path_to.c_str()));
                while (dwObjBase++ < dwObjBase + Len) {
                    if (*reinterpret_cast<std::uint16_t*>(dwObjBase + 0x00) == 0x06C7 &&
                        *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x06) == 0x8689 &&
                        *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x0C) == 0x8689) {
                        dwObjBase += 2;
                        break;
                    }
                }
                return dwObjBase;
            }
            return std::uintptr_t(0);
        }(Len);
        return base;
    }
    static void* getFuncAddr(int VTableIndex) {
        return (*reinterpret_cast<void***>(findDevice(0x128000)))[VTableIndex];
    }
}; // class Direct3D9Wrapper

#endif // !_DIRECT3D9_WRAPPER_HPP_