using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace SYY
{
    class OkCardSDK
    {
#if DEBUG
        const string Dll_Name = "OkCardSDK_x64_d.dll";
#else
        const string Dll_Name = "OkCardSDK_x64.dll";
#endif
        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode Init(ref ulong hHandle, ref CImage cImage);

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode Release(ref ulong hHandle);

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode CaptureImage(
            ulong hHandle, 
            ref CImage cImage
            );
    }
}
