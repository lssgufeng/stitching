﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ImageStitcher
{
    static class NativeMethods
    {
        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport("kernel32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);


        [DllImport("kernel32.dll")]
        public static extern bool FreeLibrary(IntPtr hModule);       
    }

    //[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    //private delegate double findSquareRoot(double value);
    //static IntPtr pDll = NativeMethods.LoadLibrary(@"Stitching.dll");
    //static IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "findSquareRoot");
    //findSquareRoot myFindSquareRoot = (findSquareRoot)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall,
    //    typeof(findSquareRoot));
     
    class StitcherWrapper
    {
        /// <summary>
        /// Stitches two images 
        /// </summary>
        /// <param name="path1">Path of the first image(float image)</param>
        /// <param name="path2">Path of the second image(base image)</param>
        /// <param name="direction">0-> horizontal, 1->vertical</param>
        /// <param name="method">0->Knn, 1-> FLANN</param>
        [DllImport(@"Stitching(1.0.1).dll")]
        public static extern void Stitch([MarshalAs(UnmanagedType.LPStr)] string path1,
            [MarshalAs(UnmanagedType.LPStr)] string path2,int direction,int method,
            [MarshalAs(UnmanagedType.Bool)] bool crop);
        [DllImport(@"Stitching(1.0.1).dll")]
        public static extern ushort[] StitchRaw(ushort[] image1, ushort[] image2); 
    }
}
