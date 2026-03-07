#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <stddef.h>
//adapted from libnds
#define unlikely(x) __builtin_expect((x), 0)
#define likely(x) __builtin_expect(!!(x), 1)
struct bufstruct {
    char * address; 
    char * const max_address;
    int padding;
    char paddingChar;
};

static struct bufstruct * buffer=NULL;

/*THUMB_CODE*/
static __attribute__((noinline)) 
int bufPutc(char c)
{
    char * max_address=buffer->max_address;
    if (likely(buffer->address<=max_address))
    {
        *(buffer->address)=c;
        buffer->address+=1;
        return 1;
    }
    buffer->address+=1;
    return 0;
}
/*THUMB_CODE*/
static __attribute__((noinline)) 
int bufPuts(const char *str)
{
    while (*str != '\0')
    {
        int ret=bufPutc(*str);
        if (ret!=1)
            goto error;
        str+=1;
    }
    return 1;
error: 
    return 0;
}

static __attribute__((noinline)) 
int bufPrintNumUnsigned(uint32_t num, uint32_t base)
{
    // When printing the number we actually get the digits in reverse, so we
    // need a small buffer to store the number and then print it in reverse from
    // there. UINT32_MAX (4 294 967 295) fits in this buffer:
    char tmp[11];

    static const char digits_str[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    int len = 0;

    while (1)
    {
        uint32_t digit = num % base;
        num = num / base;

        tmp[len++] = digits_str[digit];

        // Check this at the end of the first iteration so that the number "0"
        // can be printed instead of not printing anything.
        if (num == 0)
            break;
    }
    int prepadding=(buffer->padding-len);
    int charsPrinted=0;
    for (int i=0; i<prepadding;i++)
    {
      charsPrinted+=bufPutc(buffer->paddingChar);
    }

    while (len > 0)
    {
        len--;
        charsPrinted+=bufPutc(tmp[len]);
    }
    buffer->padding=0;
    buffer->paddingChar=' ';
    return charsPrinted;
}

static int bufVprintf(const char *fmt, va_list args)
{
    char *buf0=buffer->address;
    int charsPrinted=0;
    while (1)
    {
        int c = *fmt++;
        if (c == '\0')
            break;

        if (c != '%')
        {
            bufPutc(c);
        }
        else
        {
            // We don't support modifiers so we only need to check one more
            // character.
            c = *fmt++;
            console_switch:
            switch (c)
            {
                case '\0':
                    goto fmtError;
                case '.':
                case '0':
                    //do something better here?
                    buffer->padding=0;
                    buffer->paddingChar='0';
                    c=*fmt;
                    fmt++;
                    __attribute__ ((fallthrough));
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    c=c-'0';
                    while (c>=0 && c<=9)
                    {
                        buffer->padding=buffer->padding*10+c;
                        c=*fmt-'0';
                        fmt++;
                    }
                    c+='0';
                    goto console_switch;
                }
                case '%':
                {
                    bufPutc('%');
                    break;
                }
                case 'c':
                {
                    int val = va_arg(args, int);
                    bufPutc(val);
                    break;
                }
                case 's':
                {
                    const char *str = va_arg(args, char *);
                    bufPuts(str);
                    break;
                }
                case 'x':
                {
                    unsigned int unum = va_arg(args, unsigned int);
                    bufPrintNumUnsigned(unum, 16);
                    break;
                }
                case 'u':
                {
                    unsigned int unum = va_arg(args, unsigned int);
                    bufPrintNumUnsigned(unum, 10);
                    break;
                }
                case 'd':
                {
                    int num = va_arg(args, int);
                    unsigned int unum;

                    if (num < 0)
                    {
                        bufPutc('-');
                        unum = -num;
                    }
                    else
                    {
                        unum = num;
                    }

                    bufPrintNumUnsigned(unum, 10);
                    break;
                }
                case 'f':
                {
                    float f0 = va_arg(args, double);
                    union {float f; uint32_t u;}fu;
                    fu.f=f0;
                    uint32_t f=fu.u;
                    if ((int)f<0) 
                        bufPutc('-');
                    f<<=1;
                    f>>=1;
                    if (unlikely((f>>23)==255))
                    {
                  		if (f<<9)
	                    {
	                        bufPuts("NaN");	
	                    }
	                    else
	                    {
	                       bufPuts("Inf");
	                    }
                        break;
                    }	
                    int shift=((f>>23)-23-127);
                    if (shift>40){
                        bufPuts("ERANGE");
                        errno=ERANGE;
                        break; //do something better here? no idea what though
                    }else if (shift<=-64){
                        bufPuts("0.000000");
                        break;
                    }
                    uint64_t mantissa=(((f<<9)>>9));
                    if (likely((f>>23)!=0))
                        mantissa+=(1<<23);
                    uint64_t ipart= shift>=0 ? mantissa <<shift : mantissa>> -shift;
                    shift+=23;
                    uint32_t fracpart=0;
                    if (shift<64)
                        fracpart= shift>=0 ? mantissa <<shift : mantissa >> -shift;
                    fracpart&=(1<<23)-1;
                    fracpart=(((uint64_t)fracpart*1000000u)+(1u<<22))/(1u<<23);
                    if (unlikely(fracpart>=1000000u))
                    {
                        ipart+=1;
                        fracpart-=1000000u;
                    }
                    //int t=buffer->padding;
                    buffer->padding=0;//todo: do something with the padding
                    int charsPrinted=0;
                    if ((ipart>>32)!=0)
                    {
                        //not equipped to handle this
                        bufPuts("ERANGE");
                        errno=ERANGE;
                        break;
                    }
                    charsPrinted+=bufPrintNumUnsigned((uint32_t)ipart, 10);
                    buffer->padding=6;                    
                    buffer->paddingChar='0';
                    bufPutc('.');
                    bufPrintNumUnsigned((uint32_t) fracpart,10);
                    break;
                }

                default:
                {
                    goto fmtError;
                }
            }//switch
        }//if-else
    }//while
    charsPrinted=buffer->address-buf0;
    if (likely(buffer->address<=buffer->max_address))
    {
        *(buffer->address)='\0';
        return charsPrinted;
    }
    *(buffer->max_address)='\0';
    return -charsPrinted;
    fmtError:
    *(buffer->max_address)='\0';
    return -1;        
}
//int bufVprintf(char * buf, char * max_address , const char *fmt, va_list args);
//int consolePrintf(const char *fmt, ...)
int snprintf_arm7(char * s, size_t n , const char * fmt, ...)
{
    struct bufstruct * t=buffer; 
    //if 32-bit access wasnt atomic this would require a critical section
    struct bufstruct stackBuffer={.address=s, .max_address= s+n-1, .padding=0, .paddingChar=' '};   
    buffer=&stackBuffer;
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = bufVprintf(fmt, va);
    va_end(va);
    buffer=t;
    return ret;
//returns -1 on format string error
//returns negative number of characters it tried
//to write if buffer wasnt big enough
}

