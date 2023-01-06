#include "texture.h"

#include "../../menu/ImGui/imgui_impl_dx9.h"
#include <d3d9.h>
#include "../../../dependencies/utilities/renderer/renderer.hpp"
void* ImGui_CreateTextureRGBA ( int width, int height, const unsigned char* data ) {
    IDirect3DTexture9* tempTexture;
    if ( render::device->CreateTexture ( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &tempTexture, nullptr ) != D3D_OK )
        return nullptr;

    D3DLOCKED_RECT lockedRect;
    if ( tempTexture->LockRect ( 0, &lockedRect, nullptr, D3DLOCK_DISCARD ) != D3D_OK ) {
        tempTexture->Release ();
        return nullptr;
    }

    for ( int y = 0; y < height; ++y ) {
        const auto src = reinterpret_cast<const std::uint32_t*>( data + width * 4 * y );
        const auto dest = reinterpret_cast<std::uint32_t*>( (unsigned char*) lockedRect.pBits + lockedRect.Pitch * y );

        // RGBA --> BGRA
        for ( int i = 0; i < width; ++i ) {
            auto color = src[ i ];
            color = ( color & 0xFF00FF00 ) | ( ( color & 0xFF0000 ) >> 16 ) | ( ( color & 0xFF ) << 16 );
            dest[ i ] = color;
        }
    }

    tempTexture->UnlockRect ( 0 );

    IDirect3DTexture9* texture;
    if ( render::device->CreateTexture ( width, height, 1, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr ) != D3D_OK ) {
        tempTexture->Release ();
        return nullptr;
    }

    render::device->UpdateTexture ( tempTexture, texture );
    tempTexture->Release ();

    return texture;
}
void Texture::init ( int width, int height, const std::uint8_t* data ) noexcept {
    texture = ImGui_CreateTextureRGBA ( width, height, data );
}

void Texture::clear () noexcept {
   texture = nullptr;
}
