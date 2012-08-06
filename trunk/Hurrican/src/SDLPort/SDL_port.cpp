
#include "SDL_port.h"

#if defined(USE_PVRTC)
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03

#define PVRTC_HEADER_SIZE 52

enum {
    PVRTC_RGB_2BPP=0,
    PVRTC_RGBA_2BPP,
    PVRTC_RGB_4BPP,
    PVRTC_RGBA_4BPP
};
#endif

GLenum MatrixMode = 0;
D3DXMATRIXA16 g_matView;
std::vector<GLuint> textures;

#ifndef __WIN32__
void DeleteFile( const char* filename )
{
}
#endif

bool isPowerOfTwo(int x){return ((x != 0) && !(x & (x - 1)));}

int nextPowerOfTwo(int x)
{
    double logbase2 = log(x) / log(2);
    return (int)round(pow(2,ceil(logbase2)));
}

SDL_Surface* loadImage( const char* path, uint32_t size )
{
    SDL_Rect rawDimensions;
    SDL_Surface* surface = NULL;	// This surface will tell us the details of the image
    SDL_Surface* final = NULL;

    if (size <= 0)  // Load from file
    {
        surface = IMG_Load(path);
    }
    else            // Load from memory
    {
        SDL_RWops* sdl_rw = SDL_RWFromConstMem( (const void*)path, size );

        if (sdl_rw != NULL)
        {
            surface = IMG_Load_RW( sdl_rw, 1 );
        }
        else
        {
            printf( "ERROR Texture: Failed to load texture: %s\n", SDL_GetError() );
            return NULL;
        }
    }

    if (surface != NULL)
    {
        //  Store dimensions of original RAW surface
        rawDimensions.x = surface->w;
        rawDimensions.y = surface->h;

#if defined(USE_GLES1)
        //  Check if surface is PoT
        if (!isPowerOfTwo(surface->w))
            rawDimensions.x = nextPowerOfTwo(surface->w);
        if (!isPowerOfTwo(surface->h))
            rawDimensions.y = nextPowerOfTwo(surface->h);
#endif

        final = SDL_CreateRGBSurface(SDL_SWSURFACE, rawDimensions.x, rawDimensions.y, 32,
        #if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks
            0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
        #else
            0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
        #endif
            );

        //  check if original image uses an alpha channel
        if (!(surface->flags & SDL_SRCALPHA)) {
            // if no alpha use MAGENTA and key it out.
            SDL_SetColorKey( surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( surface->format, 255, 0, 255 ) );
        } else {
            SDL_SetAlpha( surface, 0, 0 );
        }

        SDL_BlitSurface( surface, 0, final, 0 );
        SDL_FreeSurface( surface );

        return final;
    }
    else
        return NULL;
}

GLuint loadTexture( const char* path, SDL_Rect& dims, uint32_t size )
{
    int x, y;
    int xh, yh = 0;
    SDL_Surface* temp = NULL;
    SDL_Surface* surface = NULL;
    GLuint texture;			// This is a handle to our texture object

#if defined(USE_PVRTC)
    uint8_t* pvrtc_buffer = NULL;
    uint32_t* pvrtc_buffer32 = NULL;
    uint32_t pvrtc_format, pvrtc_filesize, pvrtc_size, pvrtc_width, pvrtc_height, pvrtc_depth, pvrtc_bitperpixel;
    std::string filename = path;

    filename = path + std::string(".pvr");

    pvrtc_format = pvrtc_filesize = pvrtc_width = pvrtc_height = 0;

    pvrtc_buffer = LoadFileToMemory( filename, pvrtc_filesize );

    if (pvrtc_buffer != NULL)
    {
        pvrtc_buffer32  = (uint32_t*)pvrtc_buffer;

        switch ( pvrtc_buffer32[2] )
        {
            case PVRTC_RGB_2BPP:
                pvrtc_format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            case PVRTC_RGBA_2BPP:
                pvrtc_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                pvrtc_bitperpixel = 2;
                break;
            case PVRTC_RGB_4BPP:
                pvrtc_format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            case PVRTC_RGBA_4BPP:
                pvrtc_format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                pvrtc_bitperpixel = 4;
                break;
            default:
                printf( "ERROR Unknown PVRTC format %X\n",  pvrtc_buffer32[2] );
                delete [] pvrtc_buffer;
        }

        pvrtc_height    = pvrtc_buffer32[6];
        pvrtc_width     = pvrtc_buffer32[7];
        pvrtc_depth     = pvrtc_buffer32[8];
    }
    else
    {
#endif
        temp = loadImage( path, size );

        if (temp != NULL)
        {
#if defined(USE_GLES1)
            if ((temp->h >= 512 || temp->w >= 512) && strstr(path, "font") == NULL )
            {
                surface = SDL_CreateRGBSurface(SDL_SWSURFACE, temp->w/2, temp->h/2, 32,
                #if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks
                    0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
                #else
                    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
                #endif
                    );

                xh = 0;
                yh = 0;
                for (y=0; y<temp->h; y+=2)
                {
                    for (x=0; x<temp->w; x+=2)
                    {
                        if (xh < surface->w && yh < surface->h)
                            putpixel( surface, xh, yh, getpixel( temp, x, y) );
                        else
                            printf( "error tex shrink overflow\n" );

                        xh++;
                    }
                    yh++;
                    xh = 0;
                }

                dims.w = temp->w;
                dims.h = temp->h;

                SDL_FreeSurface( temp );
            }
            else
            {
#endif
                surface = temp;

                dims.w = surface->w;
                dims.h = surface->h;
#if defined(USE_GLES1)
            }
#endif
        }
        else
            return 0;
#if defined(USE_PVRTC)
    }
#endif

    //tex_mem_size += surface->w * surface->h * surface->format->BytesPerPixel;
    //printf( "%d bytes in textures\n", tex_mem_size );

#if defined(USE_PVRTC)
    if (surface != NULL || pvrtc_buffer != NULL) {
#else
    if (surface != NULL) {
#endif
        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

#if defined(USE_PVRTC)
        if (pvrtc_buffer != NULL)
        {
            dims.w = pvrtc_width;
            dims.h = pvrtc_height;

            pvrtc_size = (pvrtc_width * pvrtc_height * pvrtc_depth * pvrtc_bitperpixel) / 8;


            printf( "Loaded PVRTC type %d for %s\n", pvrtc_buffer32[2], filename.c_str() );

#if defined(DEBUG)
            printf( "Version %X\nFlags %d\nPFormatA %d\nPFormatB %d\nColorS %d\nChanType %d\nHeight %d\nWidth %d\nDepth %d\nNumSurf %d\nNumFaces %d\nMipmap %d\nMeta %d\n", pvrtc_buffer32[0], pvrtc_buffer32[1], pvrtc_buffer32[2], pvrtc_buffer32[3],
                    pvrtc_buffer32[4], pvrtc_buffer32[5], pvrtc_buffer32[6], pvrtc_buffer32[7], pvrtc_buffer32[8],
                     pvrtc_buffer32[9], pvrtc_buffer32[10], pvrtc_buffer32[11], pvrtc_buffer32[12] );
#endif

            glCompressedTexImage2D( GL_TEXTURE_2D, 0, pvrtc_format,
                                    pvrtc_width, pvrtc_height, 0, pvrtc_size, pvrtc_buffer+PVRTC_HEADER_SIZE );
        }
        else
        {
#endif
            // Edit the texture object's image data using the information SDL_Surface gives us
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                          GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
#if defined(USE_PVRTC)
        }
#endif

#if defined(_DEBUG)
        int error = glGetError();
        if (error != 0)
            printf( "GL Tex Error %X %s\n", error, path );
#endif
    }
    else {
        printf("SDL could not load image: %s\n", SDL_GetError());
        return 0;
    }

    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }

#if defined(USE_PVRTC)
    if (pvrtc_buffer != NULL) {
        delete [] pvrtc_buffer;
    }
#endif

    textures.push_back( texture ); /* save a ref for deletion leter */

    return texture;
}

void delete_texture( GLuint texture )
{
    uint16_t i;

    for (i=0; i<textures.size(); i++)
    {
        if (texture == textures.at(i))
        {
            glDeleteTextures( 1, &textures.at(i) );
            textures.at(i) = 0;
        }
    }
}

void delete_textures( void )
{
    uint16_t i;

    for (i=0; i<textures.size(); i++)
    {
        if (textures.at(i) != 0)
        {
            glDeleteTextures( 1, &textures.at(i) );
            textures.at(i) = 0;
        }
    }

    textures.clear();
}

void D3DXMatrixIdentity( D3DXMATRIXA16* m )
{
    m->identity();
}

void fopen_s( FILE** file, const char* path, const char* mode )
{
    *file = fopen( path, mode  );
}

void strcat_s( const char* dst, const char* src )
{
    strcat( (char*)dst, src );
}

void strcat_s( const char* dst, uint32_t size, const char* src )
{
    strncat( (char*)dst, src, size );
}

void strncat_s( const char* dst, const char* src, uint32_t size )
{
    strncat( (char*)dst, src, size );
}

void strcpy_s( const char* dst, int size, const char* src )
{
    strncpy( (char*)dst, src, size);
}

void strcpy_s( const char* dst, const char* src )
{
    strcpy( (char*)dst, src );
}

void strncpy_s( const char* dst, const char* src, int size )
{
    strncpy( (char*)dst, src, size );
}

void fprintf_s( FILE* file, const char* src)
{
    fprintf( file, src );
}

void _itoa_s( int value, const char* dst, int size )
{
    sprintf( (char*)dst, "%d", value );
}

#define SWAP(T, a, b) \
    do { T save = (a); (a) = (b); (b) = save; } while (0)

char* _strrev( char *s )
{
    size_t len = strlen(s);

    if (len > 1) {
        char *a = s;
        char *b = s + len - 1;

        for (; a < b; ++a, --b)
            SWAP(char, *a, *b);
    }

    return s;
}

uint32_t getpixel( SDL_Surface *surface, int16_t x, int16_t y )
{
    int16_t bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(uint16_t *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(uint32_t *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel( SDL_Surface *surface, int16_t x, int16_t y, uint32_t pixel )
{
    int16_t bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(uint16_t *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
        default:
            *(uint32_t *)p = pixel;
            break;
    }
}

void int_to_rgb( uint32_t color, SDL_Color& components )
{
    SDL_GetRGB(color, SDL_GetVideoSurface()->format, &components.r, &components.g, &components.b);
}

void matrixmode( GLenum mode )
{
    if (MatrixMode != mode)
    {
        glMatrixMode( mode );
        MatrixMode = mode;
    }
}

uint8_t* LoadFileToMemory( const std::string& name, uint32_t& size )
{
    std::fstream file;
    uint8_t* buffer;

    file.open( name.c_str(), std::ios_base::in );
    if (file.is_open() == 0)
    {
        printf( "Error file operation: File: %s\n", name.c_str() );
        return NULL;
    }

    file.seekg( 0, std::ios::end );
    size = file.tellg();
    file.seekg( 0, std::ios::beg );

    buffer = new uint8_t[size+1];
    file.read( (char*)buffer, size );
    buffer[size] = '\0';

    file.close();

    return buffer;
}
