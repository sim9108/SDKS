src\SDL_stdlib

memcpy과 memset 부분 주석처리 필요!!

#if _MSC_VER >= 1400
/*extern void* memcpy(void* dst, const void* src, size_t len);
#pragma intrinsic(memcpy)

#pragma function(memcpy)
void *
memcpy(void *dst, const void *src, size_t len)
{
    return SDL_memcpy(dst, src, len);
}

extern void *memset(void* dst, int c, size_t len);
#pragma intrinsic(memset)

#pragma function(memset)
void *
memset(void *dst, int c, size_t len)
{
    return SDL_memset(dst, c, len);
}*/
#endif /* _MSC_VER >= 1400 */