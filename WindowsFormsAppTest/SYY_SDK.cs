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

    public static class Define {
        public const int BUAnalysisResultArrayMaxLen = 128;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Rect
    {
        public int x, y, w, h;
    }

    public struct CImage
    {
        public IntPtr pData;
        public int nWidth;
        public int nHeight;
        public int nChannels;
    }

	public enum LessionGrading
	{
		LG1a = 0,
		LG_OTHER = 1,
	};
	public enum LessionType
	{
		NO_LESSION = 0,
		LESSION = 1,
	};

    [StructLayout(LayoutKind.Sequential)]
    public struct BUAnalysisResult
    {
		public Rect rCropRect;				// 有效图片区域

		public LessionGrading nGrading;	// 病况分级

		public int nLessionsCount;		// 病灶数量

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.BUAnalysisResultArrayMaxLen)]
		public Rect[] LessionRects;		// 病灶区域

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.BUAnalysisResultArrayMaxLen)]
		public float[] LessionConfidences;	// 病灶置信值

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Define.BUAnalysisResultArrayMaxLen)]
		public LessionType[] LessionTypes;	// 病灶类型
    }

	public enum ErrorCode
	{
		SYY_NO_ERROR = 0,					// 返回成功

		// SDK Init
		SYY_SDK_REPEAT_INIT = 1,			// 重复初始化sdk
		SYY_SDK_NO_INIT = 2,				// 没有初始化sdk

		// ERROR
		SYY_SYS_ERROR = 10,					// 系统错误
		SYY_LOG_INIT_NO_PERMISSION = 11,	// 日志初始化失败，程序无权限
		SYY_NO_IMPLEMENTION = 11,			// 算法未实现

        // Video
		SYY_VIDEO_END_FRAME = 20,			// 视频加载到最后一帧
	};

    public class SDK
    {

#if DEBUG
        const string Dll_Name = "MedicalAnalysisSDK_x64_d.dll";
#else
        const string Dll_Name = "MedicalAnalysisSDK_x64.dll";
#endif

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode InitSDK();

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode ReleaseSDK();

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode InitBUAnalysis(
            ref ulong hHandle
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode ReleaseBUAnalysis(
            ref ulong hHandle
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode ExecuteBUAnalysisFromImage(
            ulong hHandle, 
            CImage cImage,
            ref BUAnalysisResult pResult
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode DrawResult2Image(
            ref CImage cImage,
            BUAnalysisResult pResult
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode LoadVideo(
            IntPtr pVideoFile,
            int nVideoFileStrLength,
            ref ulong hHandle,
            ref CImage cImage
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode GetVideoFrame(
            ulong hHandle,
            ref CImage cImage 
            );

        [DllImport(Dll_Name, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public extern static ErrorCode ReleaseVideo(
            ref ulong hHandle
            );

        public static bool CvtBitmap2CImage(Bitmap bitmap, ref CImage image)
        {
            switch(bitmap.PixelFormat)
            {
                case PixelFormat.Format24bppRgb:
                    image.nChannels = 3;
                    break;
                case PixelFormat.Format32bppRgb:
                    image.nChannels = 4;
                    break;
                case PixelFormat.Format8bppIndexed:
                    image.nChannels = 1;
                    break;
                default:
                    return false;
            }
            image.nWidth = bitmap.Width;
            image.nHeight = bitmap.Height;

            if (bitmap.Width % 4 != 0)
            {
                int w = bitmap.Width - bitmap.Width % 4;
                float h = (float)bitmap.Height / (float)bitmap.Width * (float)w;
                bitmap = bitmap.GetThumbnailImage(w, (int)h, null, IntPtr.Zero) as Bitmap;
            }
            BitmapData bmpData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                ImageLockMode.ReadOnly, bitmap.PixelFormat);

            image.pData = bmpData.Scan0;

            bitmap.UnlockBits(bmpData);

            return true;
        }
    }

}
