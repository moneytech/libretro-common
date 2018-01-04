/* Copyright  (C) 2010-2017 The RetroArch team
*
* ---------------------------------------------------------------------------------------
* The following license statement only applies to this file (file_stream_transforms.c).
* ---------------------------------------------------------------------------------------
*
* Permission is hereby granted, free of charge,
* to any person obtaining a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string.h>
#include <stdarg.h>

#include <libretro.h>
#include <streams/file_stream.h>

RFILE* rfopen(const char *path, const char *mode)
{
   RFILE          *output  = NULL;
   unsigned int retro_mode = RETRO_VFS_FILE_ACCESS_READ;
   bool position_to_end    = false;

   if (strstr(mode, "w"))
      retro_mode = RETRO_VFS_FILE_ACCESS_WRITE;

   if (strstr(mode, "a"))
   {
      retro_mode = RETRO_VFS_FILE_ACCESS_READ_WRITE | 
         RETRO_VFS_FILE_ACCESS_UPDATE_EXISTING;
      position_to_end = true;
   }

   if (strstr(mode, "+"))
   {
      retro_mode = RETRO_VFS_FILE_ACCESS_READ_WRITE;
      if (strstr(mode, "r"))
         retro_mode = retro_mode & RETRO_VFS_FILE_ACCESS_UPDATE_EXISTING;
   }

   output = filestream_open(path, retro_mode,
         RETRO_VFS_FILE_ACCESS_HINT_NONE);
   if (output && position_to_end)
      filestream_seek(output, 0, RETRO_VFS_SEEK_POSITION_END);

   return output;
}

int rfclose(RFILE* stream)
{
   return filestream_close(stream);
}

long rftell(RFILE* stream)
{
   return filestream_tell(stream);
}

int rfseek(RFILE* stream, long offset, int origin)
{
   int seek_position = -1;
   switch (origin)
   {
      case SEEK_SET:
         seek_position = RETRO_VFS_SEEK_POSITION_START;
         break;
      case SEEK_CUR:
         seek_position = RETRO_VFS_SEEK_POSITION_CURRENT;
         break;
      case SEEK_END:
         seek_position = RETRO_VFS_SEEK_POSITION_END;
         break;
   }

   return filestream_seek(stream, offset, seek_position);
}

size_t rfread(void* buffer,
   size_t elem_size, size_t elem_count, RFILE* stream)
{
   return filestream_read(stream, buffer, elem_size * elem_count);
}

char *rfgets(char *buffer, int maxCount, RFILE* stream)
{
   return filestream_gets(stream, buffer, maxCount);
}

int rfgetc(RFILE* stream)
{
	return filestream_getc(stream);
}

size_t rfwrite(void const* buffer,
   size_t elem_size, size_t elem_count, RFILE* stream)
{
   return filestream_write(stream, buffer, elem_size * elem_count);
}

int rfputc(int character, RFILE * stream)
{
    return filestream_putc(stream, character);
}

int rfprintf(RFILE * stream, const char * format, ...)
{
   int result;
	va_list vl;
	va_start(vl, format);
	result = filestream_vprintf(stream, format, vl);
	va_end(vl);
	return result;
}

int rferror(RFILE* stream)
{
   return filestream_error(stream);
}

int rfeof(RFILE* stream)
{
   return filestream_eof(stream);
}
